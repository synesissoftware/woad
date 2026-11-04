/* /////////////////////////////////////////////////////////////////////////
 * File:    test/component/mintty_pty/entry.c
 *
 * Purpose: Component test for mintty (Git Bash / MSYS2 / Cygwin) PTY pipe
 *          recognition. Only the public predicates are used. On Win32
 *          (not a Cygwin/MSYS-linked build) named pipes are created and
 *          the fd, stream, and handle results are checked. Elsewhere a
 *          disk file is checked to be plain.
 *
 * Created: 5th November 2026
 * Updated: 4th November 2026
 *
 * Home:    https://github.com/synesissoftware/woad/
 *
 * Copyright (c) 2026, Matthew Wilson and Synesis Information Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer;
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution;
 * - Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ////////////////////////////////////////////////////////////////////// */


#if !defined(_WIN32) &&\
    !defined(_POSIX_C_SOURCE)
# define _POSIX_C_SOURCE                                    200809L
#endif

#include <woad/woad.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if defined(_WIN32) &&\
    !defined(__CYGWIN__) &&\
    !defined(__MSYS__)

# if !defined(_WIN32_WINNT) || _WIN32_WINNT < 0x0600
#  undef _WIN32_WINNT
#  define _WIN32_WINNT 0x0600
# endif
# if !defined(WINVER) || WINVER < 0x0600
#  undef WINVER
#  define WINVER 0x0600
# endif
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <fcntl.h>
# include <io.h>
# include <windows.h>
#endif


/* /////////////////////////////////////////////////////////////////////////
 * test infrastructure
 */

static int
fail_(
    char const*  expr
,   char const*  file
,   int          line
)
{
    fprintf(stderr, "%s:%d: assertion failed: %s\n", file, line, expr);

    return EXIT_FAILURE;
}

#define ASSERT_TRUE_(expr)                                  \
                                                            \
    do                                                      \
    {                                                       \
        if (!(expr))                                        \
        {                                                   \
            return fail_(#expr, __FILE__, __LINE__);        \
        }                                                   \
    } while (0)


static void
unset_env_(
    char const*  name
)
{
#ifdef _WIN32

    (void)_putenv_s(name, "");
#else

    (void)unsetenv(name);
#endif

    woad_refresh();
}

static void
clear_policy_env_(void)
{
    unset_env_("CLICOLOR");
    unset_env_("CLICOLOR_FORCE");
    unset_env_("FORCE_COLOR");
    unset_env_("NO_COLOR");
    unset_env_("TERM");
    unset_env_("WOAD_COLOR");
    unset_env_("WOAD_COLOUR");
    unset_env_("WOAD_SET_CONSOLE_MODE");
}

static int
test_plain_file_(void)
{
    FILE* disk;

    clear_policy_env_();

    disk = tmpfile();

    if (NULL == disk)
    {
        return fail_("tmpfile", __FILE__, __LINE__);
    }

    if (0 != woad_stream_is_tty(disk) ||
        0 != woad_stream_supports_colour(disk))
    {
        fclose(disk);

        return fail_("disk file", __FILE__, __LINE__);
    }

    fclose(disk);

    return EXIT_SUCCESS;
}


#if defined(_WIN32) &&\
    !defined(__CYGWIN__) &&\
    !defined(__MSYS__)

static void
append_w_(
    wchar_t*        dst
,   size_t          cap
,   wchar_t const*  src
)
{
    size_t n = 0;

    while (L'\0' != dst[n])
    {
        ++n;
    }

    while (L'\0' != *src &&
           (n + 1) < cap)
    {
        dst[n++] = *src++;
    }

    dst[n] = L'\0';
}

static void
append_u_(
    wchar_t*        dst
,   size_t          cap
,   unsigned long   value
)
{
    wchar_t rev[16];
    int     n = 0;
    size_t  used = 0;

    if (0ul == value)
    {
        rev[n++] = L'0';
    }

    while (0ul != value &&
           n < 16)
    {
        rev[n++] = (wchar_t)(L'0' + (value % 10ul));
        value /= 10ul;
    }

    while (L'\0' != dst[used])
    {
        ++used;
    }

    while (n > 0 &&
           (used + 1) < cap)
    {
        dst[used++] = rev[--n];
    }

    dst[used] = L'\0';
}

static unsigned s_pipe_seq;

static void
make_pipe_name_(
    wchar_t*        dst
,   size_t          cap
,   wchar_t const*  stem
,   wchar_t const*  tail
)
{
    dst[0] = L'\0';

    append_w_(dst, cap, L"\\\\.\\pipe\\");
    append_w_(dst, cap, stem);
    append_u_(dst, cap, GetCurrentProcessId());
    append_u_(dst, cap, s_pipe_seq++);
    append_w_(dst, cap, tail);
}

static void
note_os_name_(
    HANDLE          h
)
{
    struct
    {
        DWORD FileNameLength;
        WCHAR FileName[256];

    } info;

    fputs("os pipe name: ", stderr);

    if (!GetFileInformationByHandleEx(h, FileNameInfo, &info, sizeof(info)))
    {
        fprintf(stderr, "(GetFileInformationByHandleEx failed %lu)\n", GetLastError());

        return;
    }

    {
        DWORD const nchars = info.FileNameLength / (DWORD)sizeof(WCHAR);
        DWORD       i;

        for (i = 0; i < nchars && i < 255; ++i)
        {
            unsigned const ch = (unsigned)info.FileName[i];

            if (ch >= 32u &&
                ch < 127u)
            {
                fputc((int)ch, stderr);
            }
            else
            {
                fprintf(stderr, "\\x%04x", ch);
            }
        }
    }

    fputc('\n', stderr);
}

static int
close_pair_(
    HANDLE          server
,   HANDLE          client
)
{
    if (NULL != server &&
        INVALID_HANDLE_VALUE != server)
    {
        CloseHandle(server);
    }

    if (NULL != client &&
        INVALID_HANDLE_VALUE != client)
    {
        CloseHandle(client);
    }

    return EXIT_SUCCESS;
}

/* expect_terminal: public fd/stream report a terminal and colour; the
 * console-handle predicate stays 0, while handle colour follows the pipe.
 */
static int
probe_named_pipe_(
    wchar_t const*  name
,   int             expect_terminal
)
{
    HANDLE server = CreateNamedPipeW(
        name
    ,   PIPE_ACCESS_DUPLEX
    ,   PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT
    ,   1
    ,   256
    ,   256
    ,   0
    ,   NULL
    );
    HANDLE client;
    int    fd;
    FILE*  stm;

    if (INVALID_HANDLE_VALUE == server)
    {
        fprintf(stderr, "CreateNamedPipeW failed %lu\n", GetLastError());

        return EXIT_FAILURE;
    }

    client = CreateFileW(
        name
    ,   GENERIC_READ | GENERIC_WRITE
    ,   0
    ,   NULL
    ,   OPEN_EXISTING
    ,   0
    ,   NULL
    );

    if (INVALID_HANDLE_VALUE == client)
    {
        fprintf(stderr, "CreateFileW failed %lu\n", GetLastError());
        CloseHandle(server);

        return EXIT_FAILURE;
    }

    if ((0 != woad_console_handle_is_tty(server)) ||
        (0 != woad_console_handle_is_tty(client)))
    {
        note_os_name_(client);
        close_pair_(server, client);

        return fail_("console is_tty on a pipe", __FILE__, __LINE__);
    }

    if (expect_terminal)
    {
        if (0 == woad_console_handle_supports_colour(client))
        {
            note_os_name_(client);
            close_pair_(server, client);

            return fail_("handle colour on mintty pipe", __FILE__, __LINE__);
        }
    }
    else
    {
        if (0 != woad_console_handle_supports_colour(client))
        {
            note_os_name_(client);
            close_pair_(server, client);

            return fail_("handle colour on ordinary pipe", __FILE__, __LINE__);
        }
    }

    fd = _open_osfhandle((intptr_t)client, _O_RDONLY | _O_BINARY);

    if (-1 == fd)
    {
        fprintf(stderr, "_open_osfhandle failed\n");
        close_pair_(server, client);

        return EXIT_FAILURE;
    }

    /* client is now owned by fd */

    stm = _fdopen(fd, "rb");

    if (NULL == stm)
    {
        _close(fd);
        CloseHandle(server);

        return fail_("_fdopen", __FILE__, __LINE__);
    }

    if (expect_terminal)
    {
        if (0 == woad_fd_is_tty(fd) ||
            0 == woad_stream_is_tty(stm) ||
            0 == woad_fd_supports_colour(fd) ||
            0 == woad_stream_supports_colour(stm))
        {
            note_os_name_((HANDLE)_get_osfhandle(fd));
            fclose(stm);
            CloseHandle(server);

            return fail_("fd/stream mintty pipe", __FILE__, __LINE__);
        }
    }
    else
    {
        if (0 != woad_fd_is_tty(fd) ||
            0 != woad_stream_is_tty(stm) ||
            0 != woad_fd_supports_colour(fd) ||
            0 != woad_stream_supports_colour(stm))
        {
            note_os_name_((HANDLE)_get_osfhandle(fd));
            fclose(stm);
            CloseHandle(server);

            return fail_("fd/stream ordinary pipe", __FILE__, __LINE__);
        }
    }

    fclose(stm);
    CloseHandle(server);

    return EXIT_SUCCESS;
}

static int test_policy_on_pty_(void);

static int
test_windows_pipes_(void)
{
    wchar_t name[160];
    HANDLE  anon_read = NULL;
    HANDLE  anon_write = NULL;
    int     rc;
    FILE*   disk;

    clear_policy_env_();

    make_pipe_name_(name, 160, L"msys-woad", L"-pty0-to-master");
    rc = probe_named_pipe_(name, 1);
    if (EXIT_SUCCESS != rc) return rc;

    make_pipe_name_(name, 160, L"cygwin-woad", L"-pty1-from-master");
    rc = probe_named_pipe_(name, 1);
    if (EXIT_SUCCESS != rc) return rc;

    make_pipe_name_(name, 160, L"msys-woad", L"-pty2-to-master-nat");
    rc = probe_named_pipe_(name, 1);
    if (EXIT_SUCCESS != rc) return rc;

    make_pipe_name_(name, 160, L"woadplain", L"");
    rc = probe_named_pipe_(name, 0);
    if (EXIT_SUCCESS != rc) return rc;

    make_pipe_name_(name, 160, L"msys-woad", L"-pty-to-master");
    rc = probe_named_pipe_(name, 0);
    if (EXIT_SUCCESS != rc) return rc;

    make_pipe_name_(name, 160, L"msys-woad", L"-pty0-too-master");
    rc = probe_named_pipe_(name, 0);
    if (EXIT_SUCCESS != rc) return rc;

    make_pipe_name_(name, 160, L"msys-woad", L"-pty0x-to-master");
    rc = probe_named_pipe_(name, 0);
    if (EXIT_SUCCESS != rc) return rc;

    make_pipe_name_(name, 160, L"msys-woad", L"-pty0-from-slave");
    rc = probe_named_pipe_(name, 0);
    if (EXIT_SUCCESS != rc) return rc;

    make_pipe_name_(name, 160, L"notmsys-woad", L"-pty0-to-master");
    rc = probe_named_pipe_(name, 0);
    if (EXIT_SUCCESS != rc) return rc;

    if (!CreatePipe(&anon_read, &anon_write, NULL, 0))
    {
        fprintf(stderr, "CreatePipe failed %lu\n", GetLastError());

        return EXIT_FAILURE;
    }

    if (0 != woad_console_handle_is_tty(anon_read) ||
        0 != woad_console_handle_supports_colour(anon_read))
    {
        CloseHandle(anon_read);
        CloseHandle(anon_write);

        return fail_("anonymous pipe", __FILE__, __LINE__);
    }

    CloseHandle(anon_read);
    CloseHandle(anon_write);

    disk = tmpfile();

    if (NULL == disk)
    {
        return fail_("tmpfile", __FILE__, __LINE__);
    }

    if (0 != woad_stream_is_tty(disk) ||
        0 != woad_stream_supports_colour(disk))
    {
        fclose(disk);

        return fail_("disk file", __FILE__, __LINE__);
    }

    fclose(disk);

    return test_policy_on_pty_();
}

static void
set_env_(
    char const*  name
,   char const*  value
)
{
    (void)_putenv_s(name, value);

    woad_refresh();
}

static int
test_policy_on_pty_(void)
{
    wchar_t name[160];
    HANDLE  server;
    HANDLE  client;
    int     fd;

    make_pipe_name_(name, 160, L"msys-woad", L"-pty4-to-master");

    server = CreateNamedPipeW(
        name
    ,   PIPE_ACCESS_DUPLEX
    ,   PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT
    ,   1
    ,   256
    ,   256
    ,   0
    ,   NULL
    );

    if (INVALID_HANDLE_VALUE == server)
    {
        fprintf(stderr, "CreateNamedPipeW failed %lu\n", GetLastError());

        return EXIT_FAILURE;
    }

    client = CreateFileW(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (INVALID_HANDLE_VALUE == client)
    {
        CloseHandle(server);

        return fail_("CreateFileW policy pipe", __FILE__, __LINE__);
    }

    fd = _open_osfhandle((intptr_t)client, _O_RDONLY | _O_BINARY);

    if (-1 == fd)
    {
        CloseHandle(client);
        CloseHandle(server);

        return fail_("_open_osfhandle policy pipe", __FILE__, __LINE__);
    }

    clear_policy_env_();

    if (0 == woad_fd_is_tty(fd) ||
        0 == woad_fd_supports_colour(fd))
    {
        _close(fd);
        CloseHandle(server);

        return fail_("pty colour before policy", __FILE__, __LINE__);
    }

    set_env_("TERM", "dumb");

    if (0 == woad_fd_is_tty(fd) ||
        0 != woad_fd_supports_colour(fd))
    {
        _close(fd);
        CloseHandle(server);

        return fail_("TERM=dumb", __FILE__, __LINE__);
    }

    unset_env_("TERM");
    set_env_("NO_COLOR", "1");

    if (0 == woad_fd_is_tty(fd) ||
        0 != woad_fd_supports_colour(fd))
    {
        _close(fd);
        CloseHandle(server);

        return fail_("NO_COLOR", __FILE__, __LINE__);
    }

    unset_env_("NO_COLOR");
    set_env_("WOAD_SET_CONSOLE_MODE", "never");

    if (0 == woad_fd_supports_colour(fd) ||
        0 == woad_console_handle_supports_colour((HANDLE)_get_osfhandle(fd)))
    {
        _close(fd);
        CloseHandle(server);

        return fail_("WOAD_SET_CONSOLE_MODE does not apply", __FILE__, __LINE__);
    }

    set_env_("WOAD_COLOUR", "never");

    if (0 == woad_fd_is_tty(fd) ||
        0 != woad_fd_supports_colour(fd))
    {
        _close(fd);
        CloseHandle(server);

        return fail_("WOAD_COLOUR=never", __FILE__, __LINE__);
    }

    clear_policy_env_();
    _close(fd);
    CloseHandle(server);

    return EXIT_SUCCESS;
}
#endif /* Win32 pipe tests */


/* /////////////////////////////////////////////////////////////////////////
 * entry point
 */

int main(void)
{
    int rc = test_plain_file_();

    if (EXIT_SUCCESS != rc)
    {
        return rc;
    }

#if defined(_WIN32) &&\
    !defined(__CYGWIN__) &&\
    !defined(__MSYS__)

    rc = test_windows_pipes_();

    if (EXIT_SUCCESS != rc)
    {
        return rc;
    }
#endif

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

