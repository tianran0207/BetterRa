// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "utils.hpp"

#include <math.h> // floor()
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
	#include "winapi.hpp"
	#ifndef F_OK
		#define F_OK   0x0
	#endif  /* F_OK */
#else
	#include <unistd.h>
	#include <dirent.h>
	#include <sys/stat.h>
#endif

#include "cbasetypes.hpp"
#include "showmsg.hpp"
#include "socket.hpp"

/// Dumps given buffer into file pointed to by a handle.
void WriteDump(FILE* fp, const void* buffer, size_t length)
{
	size_t i;
	char hex[48+1], ascii[16+1];

	fprintf(fp, "--- 00-01-02-03-04-05-06-07-08-09-0A-0B-0C-0D-0E-0F   0123456789ABCDEF\n");
	ascii[16] = 0;

	for( i = 0; i < length; i++ )
	{
		char c = RBUFB(buffer,i);

		ascii[i%16] = ISCNTRL(c) ? '.' : c;
		sprintf(hex+(i%16)*3, "%02X ", RBUFB(buffer,i));

		if( (i%16) == 15 )
		{
			fprintf(fp, "%03X %s  %s\n", (unsigned int)(i/16), hex, ascii);
		}
	}

	if( (i%16) != 0 )
	{
		ascii[i%16] = 0;
		fprintf(fp, "%03X %-48s  %-16s\n", (unsigned int)(i/16), hex, ascii);
	}
}


/// Dumps given buffer on the console.
void ShowDump(const void* buffer, size_t length)
{
	size_t i;
	char hex[48+1], ascii[16+1];

	ShowDebug("--- 00-01-02-03-04-05-06-07-08-09-0A-0B-0C-0D-0E-0F   0123456789ABCDEF\n");
	ascii[16] = 0;
        
	for( i = 0; i < length; i++ )
	{
		char c = RBUFB(buffer,i);

		ascii[i%16] = ISCNTRL(c) ? '.' : c;
		sprintf(hex+(i%16)*3, "%02X ", RBUFB(buffer,i));

		if( (i%16) == 15 )
		{
			ShowDebug("%03" PRIXPTR " %s  %s\n", i/16, hex, ascii);
		}
	}

	if( (i%16) != 0 )
	{
		ascii[i%16] = 0;
		ShowDebug("%03" PRIXPTR " %-48s  %-16s\n", i/16, hex, ascii);
	}
}


#ifdef WIN32

static char* checkpath(char *path, const char *srcpath)
{	// just make sure the char*path is not const
	char *p=path;
	if(NULL!=path && NULL!=srcpath)
	while(*srcpath) {
		if (*srcpath=='/') {
			*p++ = '\\';
			srcpath++;
		}
		else
			*p++ = *srcpath++;
	}
	*p = *srcpath; //EOS
	return path;
}

void findfile(const char *p, const char *pat, void (func)(const char*))
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;
	char tmppath[MAX_PATH+1];

	const char *path    = (p  ==NULL)? "." : p;
	const char *pattern = (pat==NULL)? "" : pat;

	checkpath(tmppath,path);
	if( PATHSEP != tmppath[strlen(tmppath)-1])
		strcat(tmppath, "\\*");
	else
		strcat(tmppath, "*");

	hFind = FindFirstFileA(tmppath, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(FindFileData.cFileName, ".") == 0)
				continue;
			if (strcmp(FindFileData.cFileName, "..") == 0)
				continue;

			sprintf(tmppath,"%s%c%s",path,PATHSEP,FindFileData.cFileName);

			if (FindFileData.cFileName && strstr(FindFileData.cFileName, pattern)) {
				func( tmppath );
			}


			if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				findfile(tmppath, pat, func);
			}
		}while (FindNextFileA(hFind, &FindFileData) != 0);
		FindClose(hFind);
	}
	return;
}

/**
 * Check if the path is a directory or file
 * @param filepath: Location of file
 * @return 0 = Error
 *         1 = Directory
 *         2 = File
 *         3 = File but doesn't exist
 */
int check_filepath(const char* filepath)
{
	DWORD Attribute;

	if (Attribute = GetFileAttributes(filepath)) {
		if ((Attribute&INVALID_FILE_ATTRIBUTES) && GetLastError() == ERROR_FILE_NOT_FOUND) {
			SetLastError(0);
			return 3;
		} else if (Attribute&FILE_ATTRIBUTE_DIRECTORY)
			return 1;
		else
			return 2;
	}

	return 0;
}

bool utf8_check(char* str, size_t length)
{
	if (str[0] == -17 && str[1] == -69 && str[2] == -65)
		return true;
	size_t i = 0;
	size_t nBytes = 0;
	bool IsUTF8 = true;
	size_t ascii = 0;
	unsigned char* start = (unsigned char*)str;
	unsigned char* end = (unsigned char*)str + length;
	unsigned char chr;
	while (start < end)
	{
		chr = *start;
		if ((chr & 0x80) == 0) {
			ascii++;
			start++;
		} else if (((chr << 2) & 0x80) == 0) {
			if (start >= end - 1)
				break;

			if ((start[1] & (0xC0)) != 0x80)
				return false;

			ascii++;
			start += 2;
		} else if (((chr << 3) & 0x80) == 0) {
			if (start >= end - 2)
				break;

			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
				return false;
				
			start += 3;
		} else {
			chr = *(start);
			while ((chr & 0x80) != 0) {
				chr <<= 1;
				nBytes++;
			}
			if (nBytes < 2 || nBytes > 6)
				return false;
			for (i = 1; i < nBytes; i++) {
				chr = *(start + i);
				if ((chr & 0xC0) != 0x80)
					return false;
			}
			start += nBytes;
		}
	}

	return (IsUTF8 && ascii != length);
}

size_t UTF8toGBK(char *u8str, size_t len)
{
	size_t slen, skip = 0;
	wchar_t *unicode;

	if (u8str[0] == -17 && u8str[1] == -69 && u8str[2] == -65)
		skip = 3;

	slen = MultiByteToWideChar(CP_UTF8, 0, u8str + skip, -1, NULL, 0);
	unicode = (wchar_t *)aMalloc(slen * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, u8str + skip, -1, unicode, slen);
	slen = WideCharToMultiByte(CP_ACP, 0, unicode, -1, NULL, 0, NULL, NULL);
	if (slen < len) {
		slen = WideCharToMultiByte(CP_ACP, 0, unicode, -1, u8str, slen, NULL, NULL);
	}else
		slen = len;
	
	aFree(unicode);
	return slen;
}

size_t fread_ex(void *_Buffer, size_t _ElementSize, size_t _ElementCount, FILE *_Stream)
{
#ifdef C_UTF8_CODE
	size_t len = fread(_Buffer, _ElementSize, _ElementCount, _Stream);
	char *buffer = (char *)_Buffer;
	buffer[len] = '\0';
	if (utf8_check(buffer, len))
		len = UTF8toGBK(buffer, len);
	return len;
#else //C_UTF8_CODE
	return fread(_Buffer, _ElementSize, _ElementCount, _Stream);
#endif
}

#include <string>
#define FILETMP 10
struct s_file_info {
	long filesize;
	char utf8;
}files[FILETMP];
int ptridx = 0;
char* fgets_ex(char *_Buffer, int _MaxCount, FILE *_Stream) {
#ifdef C_UTF8_CODE
	static char is_utf8 = 0;
	static long filesize = 0;
	int i;
	struct stat buf;
	fstat(fileno(_Stream), &buf);
	if (filesize != buf.st_size && buf.st_size != 0) {
		if (ftell(_Stream)) {
			ARR_FIND(0, FILETMP, i, files[i].filesize == buf.st_size);
			if (i < FILETMP)
				is_utf8 = files[i].utf8;
			else
				ShowError("fgets_ex: Not detected file encoding, this document possible gibberish.\n");
		} else {
			size_t len;
			char *buffer;
			buffer = (char*)aMalloc(buf.st_size + 1);
			len = fread(buffer, 1, buf.st_size, _Stream);
			buffer[len] = '\0';
			is_utf8 = (utf8_check(buffer, len) == true ? 1 : 0);
			aFree(buffer);
			fseek(_Stream, 0, SEEK_SET);
		}
		filesize = buf.st_size;
		files[ptridx].filesize = buf.st_size;
		files[ptridx].utf8 = is_utf8;
		if (++ptridx >= FILETMP)
			ptridx = 0;
	}
	if (is_utf8) {
		memset(_Buffer, 0, _MaxCount);
		if (fgets(_Buffer, _MaxCount, _Stream) == NULL)
			return NULL;
		UTF8toGBK(_Buffer, _MaxCount);
	}else if (fgets(_Buffer, _MaxCount, _Stream) == NULL)
		return NULL;

	return _Buffer;
#else //C_UTF8_CODE
	return fgets(_Buffer, _MaxCount, _Stream);
#endif
}

#ifndef MINICORE
int config_read_file_ex(config_t *config, const char *filename)
{
#ifdef C_UTF8_CODE
	size_t len;
	FILE *fp;
	char *buffer;
	int ref = 0;
	fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		ShowError("config_read_file_ex: File not found '%s'.\n", filename);
		return CONFIG_FALSE;
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	buffer = (char*)aMalloc(len + 1);
	fseek(fp, 0, SEEK_SET);
	len = fread_ex(buffer, 1, len, fp);
	buffer[len] = '\0';
	if (ferror(fp))
	{
		ShowError("config_read_file_ex: Failed to read file '%s' - %s\n", filename, strerror(errno));
		aFree(buffer);
		fclose(fp);
		return CONFIG_FALSE;
	}
	fclose(fp);
	ref = config_read_string(config, buffer);
	aFree(buffer);
	return ref;
#else //C_UTF8_CODE
	return config_read_file(config, filename);
#endif //C_UTF8_CODE
}
#endif //MINICORE

#else

#define MAX_DIR_PATH 2048

/**
 * Check if the path is a directory or file
 * @param filepath: Location of file
 * @return 0 = Error
 *         1 = Directory
 *         2 = File
 *         3 = Neither a file or directory
 */
int check_filepath(const char* filepath)
{
	struct stat s;

	if (stat(filepath, &s) == 0) {
		if (s.st_mode&S_IFDIR)
			return 1;
		else if (s.st_mode&S_IFREG)
			return 2;
		else
			return 3;
	}

	return 0;
}

static char* checkpath(char *path, const char*srcpath)
{	// just make sure the char*path is not const
	char *p=path;
	if(NULL!=path && NULL!=srcpath)
	while(*srcpath) {
		if (*srcpath=='\\') {
			*p++ = '/';
			srcpath++;
		}
		else
			*p++ = *srcpath++;
	}
	*p = *srcpath; //EOS
	return path;
}

void findfile(const char *p, const char *pat, void (func)(const char*))
{
	DIR* dir;					// pointer to the scanned directory.
	struct dirent* entry;		// pointer to one directory entry.
	struct stat dir_stat;       // used by stat().
	char tmppath[MAX_DIR_PATH * 2];
	char path[MAX_DIR_PATH+1]= ".";
	const char *pattern = (pat==NULL)? "" : pat;
	if(p!=NULL) strcpy(path,p);

	// open the directory for reading
	dir = opendir( checkpath(path, path) );
	if (!dir) {
		ShowError("Cannot read directory '%s'\n", path);
		return;
	}

	// scan the directory, traversing each sub-directory
	// matching the pattern for each file name.
	while ((entry = readdir(dir))) {
		// skip the "." and ".." entries.
		if (strcmp(entry->d_name, ".") == 0)
			continue;
		if (strcmp(entry->d_name, "..") == 0)
			continue;

		sprintf(tmppath,"%s%c%s",path, PATHSEP, entry->d_name);

		// check if the pattern matchs.
		if (entry->d_name[0] && strstr(entry->d_name, pattern)) {
			func( tmppath );
		}
		// check if it is a directory.
		if (stat(tmppath, &dir_stat) == -1) {
			ShowError("stat error %s\n': ", tmppath);
			continue;
		}
		// is this a directory?
		if (S_ISDIR(dir_stat.st_mode)) {
			// decent recursivly
			findfile(tmppath, pat, func);
		}
	}//end while

	closedir(dir);
}

size_t fread_ex(void *_Buffer, size_t _ElementSize, size_t _ElementCount, FILE *_Stream)
{
	return fread(_Buffer, _ElementSize, _ElementCount, _Stream);
}
char* fgets_ex(char *_Buffer, int _MaxCount, FILE *_Stream) {
	return fgets(_Buffer, _MaxCount, _Stream);
}

#ifndef MINICORE
int config_read_file_ex(config_t *config, const char *filename)
{
	return config_read_file(config, filename);
}
#endif //MINICORE

#endif

bool exists(const char* filename)
{
	return !access(filename, F_OK);
}

uint8 GetByte(uint32 val, int idx)
{
	switch( idx )
	{
	case 0: return (uint8)( (val & 0x000000FF)         );
	case 1: return (uint8)( (val & 0x0000FF00) >> 0x08 );
	case 2: return (uint8)( (val & 0x00FF0000) >> 0x10 );
	case 3: return (uint8)( (val & 0xFF000000) >> 0x18 );
	default:
#if defined(DEBUG)
		ShowDebug("GetByte: invalid index (idx=%d)\n", idx);
#endif
		return 0;
	}
}

uint16 GetWord(uint32 val, int idx)
{
	switch( idx )
	{
	case 0: return (uint16)( (val & 0x0000FFFF)         );
	case 1: return (uint16)( (val & 0xFFFF0000) >> 0x10 );
	default:
#if defined(DEBUG)
		ShowDebug("GetWord: invalid index (idx=%d)\n", idx);
#endif
		return 0;
	}
}
uint16 MakeWord(uint8 byte0, uint8 byte1)
{
	return byte0 | (byte1 << 0x08);
}

uint32 MakeDWord(uint16 word0, uint16 word1)
{
	return
		( (uint32)(word0        ) )|
		( (uint32)(word1 << 0x10) );
}

/*************************************
* Big-endian compatibility functions *
*************************************/

// Converts an int16 from current machine order to little-endian
int16 MakeShortLE(int16 val)
{
	unsigned char buf[2];
	buf[0] = (unsigned char)( (val & 0x00FF)         );
	buf[1] = (unsigned char)( (val & 0xFF00) >> 0x08 );
	return *((int16*)buf);
}

// Converts an int32 from current machine order to little-endian
int32 MakeLongLE(int32 val)
{
	unsigned char buf[4];
	buf[0] = (unsigned char)( (val & 0x000000FF)         );
	buf[1] = (unsigned char)( (val & 0x0000FF00) >> 0x08 );
	buf[2] = (unsigned char)( (val & 0x00FF0000) >> 0x10 );
	buf[3] = (unsigned char)( (val & 0xFF000000) >> 0x18 );
	return *((int32*)buf);
}

// Reads an uint16 in little-endian from the buffer
uint16 GetUShort(const unsigned char* buf)
{
	return	 ( ((uint16)(buf[0]))         )
			|( ((uint16)(buf[1])) << 0x08 );
}

// Reads an uint32 in little-endian from the buffer
uint32 GetULong(const unsigned char* buf)
{
	return	 ( ((uint32)(buf[0]))         )
			|( ((uint32)(buf[1])) << 0x08 )
			|( ((uint32)(buf[2])) << 0x10 )
			|( ((uint32)(buf[3])) << 0x18 );
}

// Reads an int32 in little-endian from the buffer
int32 GetLong(const unsigned char* buf)
{
	return (int32)GetULong(buf);
}

// Reads a float (32 bits) from the buffer
float GetFloat(const unsigned char* buf)
{
	uint32 val = GetULong(buf);
	return *((float*)(void*)&val);
}

/// calculates the value of A / B, in percent (rounded down)
unsigned int get_percentage(const unsigned int A, const unsigned int B)
{
	double result;

	if( B == 0 )
	{
		ShowError("get_percentage: divison by zero! (A=%u,B=%u)\n", A, B);
		return ~0U;
	}

	result = 100 * ((double)A / (double)B);

	if( result > UINT_MAX )
	{
		ShowError("get_percentage: result percentage too high! (A=%u,B=%u,result=%g)\n", A, B, result);
		return UINT_MAX;
	}

	return (unsigned int)floor(result);
}

uint32 get_percentage_exp(const uint64 a, const uint64 b)
{
	double result;

	if (b == 0)
	{
		ShowError("get_percentage_exp: divison by zero! (a=%" PRIu64 ",b=%" PRIu64 ")\n", a, b);
		return ~0U;
	}

	result = 100.0 * ((double)a / (double)b);

	if (result > UINT32_MAX)
	{
		ShowError("get_percentage_exp: result percentage too high! (a=%" PRIu64 ",b=%" PRIu64 ",result=%g)\n", a, b, result);
		return UINT32_MAX;
	}

	return (uint32)floor(result);
}
