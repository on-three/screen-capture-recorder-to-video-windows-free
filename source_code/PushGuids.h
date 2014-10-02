//------------------------------------------------------------------------------
// File: PushGuids.h
//
// Desc: DirectShow sample code - GUID definitions for PushSource filter set
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

#pragma once

#ifndef __PUSHGUIDS_DEFINED
#define __PUSHGUIDS_DEFINED

/*
// {9A80E195-3BBA-4821-B18B-21BB496F80F8}
DEFINE_GUID(CLSID_PushSourceBitmap, 
0x9a80e195, 0x3bba, 0x4821, 0xb1, 0x8b, 0x21, 0xbb, 0x49, 0x6f, 0x80, 0xf8);

// {CB83D662-33A5-4dbf-830C-25E52627C1C3}
DEFINE_GUID(CLSID_PushSourceBitmapSet, 
0xcb83d662, 0x33a5, 0x4dbf, 0x83, 0xc, 0x25, 0xe5, 0x26, 0x27, 0xc1, 0xc3);


*/

#ifdef _WIN64
DEFINE_GUID(CLSID_PushSourceDesktop, 
0x4ea69364, 0x2c8a, 0x4ae6, 0xa5, 0x61, 0x56, 0xe4, 0xb5, 0x4, 0x44, 0x39);

#else
// {e773243b-970f-4d79-9843-f7bb130faa0d}
DEFINE_GUID(CLSID_PushSourceDesktop, 
0xe773243b, 0x970f, 0x4d79, 0x98, 0x43, 0xf7, 0xbb, 0x13, 0x0f, 0xaa, 0xd0);
#endif

#endif
