/*
 * (C) 2022, Cornell University
 * All rights reserved.
 */

/* Author: Yunhao Zhang
 * Description: the earth kernel system calls
 */

#include <string.h>
#include "mem.h"
#include "syscall.h"

static struct syscall *sc = (struct syscall*)GRASS_SYSCALL_ARG;

static void sys_invoke() {
    *((int*)RISCV_CLINT0_MSIP_BASE) = 1;
    while (sc->type != SYS_UNUSED);
}

int sys_send(int receiver, char* msg, int size) {
    if (size > SYSCALL_MSG_LEN)
        return -1;

    sc->type = SYS_SEND;
    sc->payload.msg.receiver = receiver;
    memcpy(sc->payload.msg.msg, msg, size);
    sys_invoke();
    return sc->retval;    
}

int sys_recv(int* sender, char* buf, int size) {
    if (size > SYSCALL_MSG_LEN)
        return -1;

    sc->type = SYS_RECV;
    sys_invoke();
    memcpy(buf, sc->payload.msg.msg, size);
    *sender = sc->payload.msg.sender;
    return sc->retval;
}

void sys_exit(int status) {
	sc->type = SYS_EXIT;
	sc->payload.exit.status = status;
	sys_invoke();
}
