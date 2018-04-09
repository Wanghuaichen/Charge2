#pragma once
#include "sys.h"


#define FOTA_STAT "updates"
#define FOTA_END  "updated"

extern void SetFotaPara(u8 *buf);
extern void SetFotaSoftVision(char* vision);
extern void SetFotaIpSocket(char * socket);
extern void FotaRep(void);
extern void StartFota(u8* buf);
