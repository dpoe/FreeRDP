/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * Windows Clipboard Redirection
 *
 * Copyright 2012 Jason Champion
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __WF_CLIPRDR_H
#define __WF_CLIPRDR_H

#define CINTERFACE
#define COBJMACROS

#include <Ole2.h>
#include <ShlObj.h>

#include "wf_client.h"
#include <freerdp/log.h>

#define TAG CLIENT_TAG(WIN_CLIPRDR_TAG)
#ifdef WITH_DEBUG_CLIPRDR
#define DEBUG_CLIPRDR(fmt, ...) WLog_DBG(WIN_CLIPRDR_TAG, fmt, ## __VA_ARGS__)
#else
#define DEBUG_CLIPRDR(fmt, ...) do { } while (0)
#endif

typedef struct format_mapping formatMapping;
struct format_mapping {
	UINT32 remote_format_id;
	UINT32 local_format_id;
	void *name;					/* Unicode or ASCII characters with NULL terminate */
};

typedef struct cliprdr_context cliprdrContext;
struct cliprdr_context {
	rdpChannels *channels;

	UINT32 capabilities;

	formatMapping *format_mappings;
	int map_capacity;
	int map_size;

	UINT32 request_format;
	BOOL channel_initialized;

	HWND hwndClipboard;

	HANDLE cliprdr_thread;
	HANDLE hmem;
	HANDLE response_data_event;

	/* file clipping */
	CLIPFORMAT ID_FILEDESCRIPTORW;
	CLIPFORMAT ID_FILECONTENTS;
	CLIPFORMAT ID_PREFERREDDROPEFFECT;

	LPDATAOBJECT data_obj;
	ULONG req_fsize;
	char *req_fdata;
	HANDLE req_fevent;

	int nFiles;
	int file_array_size;
	wchar_t **file_names;
	FILEDESCRIPTORW **fileDescriptor;
};

void wf_cliprdr_init(wfContext* wfc, rdpChannels* channels);
void wf_cliprdr_uninit(wfContext* wfc);
void wf_process_cliprdr_event(wfContext* wfc, wMessage* event);
BOOL wf_cliprdr_process_selection_notify(wfContext* wfc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL wf_cliprdr_process_selection_request(wfContext* wfc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL wf_cliprdr_process_selection_clear(wfContext* wfc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL wf_cliprdr_process_property_notify(wfContext* wfc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
void wf_cliprdr_check_owner(wfContext* wfc);

int cliprdr_send_data_request(cliprdrContext *cliprdr, UINT32 format);
int cliprdr_send_lock(cliprdrContext *cliprdr);
int cliprdr_send_unlock(cliprdrContext *cliprdr);
int cliprdr_send_request_filecontents(cliprdrContext *cliprdr, void *streamid,
										int index, int flag, DWORD positionhigh,
										DWORD positionlow, ULONG request);

#endif /* __WF_CLIPRDR_H */
