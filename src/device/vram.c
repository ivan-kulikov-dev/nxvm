/* Copyright 2012-2014 Neko. */

/* VRAM is the random accessing memory module.  */

#include "../utils.h"

#include "vport.h"
#include "vmachine.h"
#include "vram.h"

t_ram vram;

/* Allocates memory for virtual machine ram */
static void allocate(t_nubitcc newsize) {
	if (newsize) {
		vram.size = newsize;
		if (vram.base) {
			FREE((void *) vram.base);
		}
		vram.base = (t_vaddrcc) MALLOC(vram.size);
		MEMSET((void *) vram.base, Zero8, vram.size);
	}
}

static void io_read_0092() {vport.iobyte = vram.flaga20 ? VRAM_FLAG_A20 : Zero8;}

static void io_write_0092() {vram.flaga20 = GetBit(vport.iobyte, VRAM_FLAG_A20);}

static void init() {
	MEMSET(&vram, Zero8, sizeof(t_ram));
	vport.in[0x0092] = (t_faddrcc) io_read_0092;
	vport.out[0x0092] = (t_faddrcc) io_write_0092;
	/* 16 MB */
	allocate(1 << 24);
}

static void reset() {
	MEMSET((void *) vram.base, Zero8, vram.size);
	vram.flaga20 = False;
}

static void refresh() {}

static void final() {
	if (vram.base) {
		FREE((void *) vram.base);
	}
}

void vramRegister() {vmachineAddMe;}

void deviceConnectRamAllocate(t_nubitcc newsize) {allocate(newsize);}