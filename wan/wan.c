/*
 * wan.c
 *
 *  Created on: May 7, 2019
 *      Author: wkr1u18
 */

#include "lorabase.h"
#include "oslmic.h"

uint32_t seqnoUp;

void init_wan() {
    //Initialise uplink counter to 0
    //In the future the value should be persistent - fetch from memory instead of setting to 0
    seqnoUp=0;
}

static void micB0 (u4_t devaddr, u4_t seqno, int dndir, int len) {
    os_clearMem(AESaux,16);
    AESaux[0]  = 0x49;
    AESaux[5]  = dndir?1:0;
    AESaux[15] = len;
    os_wlsbf4(AESaux+ 6,devaddr);
    os_wlsbf4(AESaux+10,seqno);
}

static void aes_appendMic (xref2cu1_t key, u4_t devaddr, u4_t seqno, int dndir, xref2u1_t pdu, int len) {
    micB0(devaddr, seqno, dndir, len);
    os_copyMem(AESkey,key,16);
    // MSB because of internal structure of AES
    os_wmsbf4(pdu+len, os_aes(AES_MIC, pdu, len));
}

static void aes_cipher (xref2cu1_t key, u4_t devaddr, u4_t seqno, int dndir, xref2u1_t payload, int len) {
    if( len <= 0 )
        return;
    os_clearMem(AESaux, 16);
    AESaux[0] = AESaux[15] = 1; // mode=cipher / dir=down / block counter=1
    AESaux[5] = dndir?1:0;
    os_wlsbf4(AESaux+ 6,devaddr);
    os_wlsbf4(AESaux+10,seqno);
    os_copyMem(AESkey,key,16);
    os_aes(AES_CTR, payload, len);
}

uint8_t* build_frame(uint32_t devaddr, uint8_t port_number, uint8_t * data, uint8_t dlen, uint8_t * nwkKey, uint8_t * artKey) {
    uint8_t frame [MAX_LEN_FRAME] = {0};
    int end = OFF_DAT_OPTS;
    //Here normally goes piggyback through additional fields

    frame[OFF_DAT_HDR] = HDR_FTYPE_DAUP | HDR_MAJOR_V1; //LoraWAN v1 +  sending unconfirmed data up
    frame[OFF_DAT_FCT] = (end-OFF_DAT_OPTS); //TODO: not sure whether correct frame control field - they are setting acknowledgement request, but do we need this?
    os_wlsbf4(frame+OFF_DAT_ADDR,  devaddr); //Copy the address
    os_wlsbf2(frame+OFF_DAT_SEQNO, seqnoUp);
    frame[end]= port_number;
    os_copyMem(frame+end+1, data, dlen); //Add payload to message
    //Not sure why they use seqnoUp-1 instead of just seqnoUp
    //Not sure why if writing to MAC port 0 we use nwkKey
    //Now we need to encrypt it
    aes_cipher(port_number==0 ? nwkKey : artKey,
                     devaddr, seqnoUp,
                     /*up*/0, frame+end+1, dlen);
    aes_appendMic(nwkKey, devaddr, seqnoUp, /*up*/0, frame, dlen+1);
    seqnoUp++; //Should we deal with 32-bit overflow?
    return frame;
}
