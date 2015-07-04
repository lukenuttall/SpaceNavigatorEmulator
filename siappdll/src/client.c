/*
3dxemu_win32 client library - 3dconnexion-compatible siappdll.dll replacement
Copyright (C) 2014  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <malloc.h>
#include <windows.h>
#include <winsock2.h>
#include "client.h"
#include "proto_def.h"

static int send_cmd(SOCKET s, int cmd, void *data, int data_size, struct reply **repptr);
static int read_bytes(SOCKET s, char *buf, int size);
static const char *win_strerror(int err);

int client_start(struct SiHdl *si)
{
	struct sockaddr_in sa;
	static int done_init;

	if(!done_init) {
		WSADATA junk;
		WSAStartup((2 << 8) | 2, &junk);
		done_init = 1;
	}

	if((si->sock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		fprintf(logfile, "failed to create client socket: %s\n", win_strerror(WSAGetLastError()));
		return -1;
	}

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	sa.sin_port = htons(SRV_PORT);

	if(connect(si->sock, (struct sockaddr*)&sa, sizeof sa) == -1) {
		fprintf(logfile, "failed to connect to the driver: %s\n", win_strerror(WSAGetLastError()));
		MessageBox(0, "Failed to connect to the driver!\nMake sure it's running...", "Error", MB_OK);
		client_stop(si);
		return -1;
	}

	if(send_cmd(si->sock, CMD_REGISTER, &si->opendata.win, 4, 0) == -1) {
		fprintf(logfile, "failed to register with the driver\n");
		client_stop(si);
		return -1;
	}
	fprintf(logfile, "%s: sucessfully registered with the driver\n", __FUNCTION__);

	return 0;
}

int client_stop(struct SiHdl *si)
{
	if(si) {
		if(si->sock != INVALID_SOCKET) {
			fprintf(logfile, "%s: closing driver connection\n", __FUNCTION__);
			closesocket(si->sock);
		}
		si->sock = INVALID_SOCKET;
	}
	return 0;
}

int client_get_event(struct SiHdl *si, int id, union client_event *ev)
{
	struct reply *reply = 0;
	if(send_cmd(si->sock, CMD_GET_EVENT, &id, sizeof id, &reply) == -1) {
		return -1;
	}
	memcpy(ev, reply->data, sizeof *ev);
	free(reply);
	return 0;
}

static int send_cmd(SOCKET s, int cmd, void *data, int data_size, struct reply **repptr)
{
	struct reply rhdr, *reply;
	int rsize;
	char *outbuf = alloca(data_size + 4);
	memcpy(outbuf, &cmd, 4);
	memcpy(outbuf + 4, data, data_size);

	send(s, outbuf, data_size + 4, 0);

	if(read_bytes(s, (char*)&rhdr, sizeof rhdr - 1) == -1 || rhdr.reply == REPLY_ERROR) {
		return -1;
	}

	rsize = sizeof rhdr + rhdr.data_size - 1;
	if(!(reply = malloc(rsize))) {
		fprintf(logfile, "%s: failed to allocate reply buffer (%d bytes)\n", __FUNCTION__, rsize);
		return -1;
	}
	*reply = rhdr;

	if(read_bytes(s, reply->data, reply->data_size) == -1) {
		free(reply);
		return -1;
	}

	if(!repptr) {
		free(reply);
		return 0;
	}
	*repptr = reply;
	return 0;
}

static int read_bytes(SOCKET s, char *buf, int size)
{
	int rdbytes = 0;
	do {
		int res = recv(s, buf + rdbytes, size - rdbytes, 0);
		if(res == -1) {
			return -1;
		}
		rdbytes += res;
	} while(rdbytes < size);

	return 0;
}

static const char *win_strerror(int err)
{
	static char msgbuf[512];
	char *endp;

	if(!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, err, 0, msgbuf, sizeof msgbuf, 0)) {
		return "<unknown error>";
	}

	if(!(endp = strrchr(msgbuf, '\n'))) {
		endp = msgbuf + strlen(msgbuf);
	}
	sprintf(endp, " (%d)\n", err);
	return msgbuf;
}
