#include <minix/ipcconst.h>
#include <machine/asm.h>

// CPUTwo IPC register convention:
//   r0 = IPCVEC_INTR (33) -- identifies this as an IPC syscall
//   r1 = call_nr (SEND, RECEIVE, etc.) -- becomes do_ipc arg0
//   r2 = src_dst_e                     -- becomes do_ipc arg1
//   r3 = m_ptr                         -- becomes do_ipc arg2
// Kernel mpx.S ipc_entry does: do_ipc(user_r1, user_r2, user_r3)

// _ipc_send_intr(dest, msg_ptr)
// C args: r0=dest, r1=msg_ptr
ENTRY(_ipc_send_intr)
	mov	r3, r1			// r3 = msg_ptr
	mov	r2, r0			// r2 = dest (src_dst_e)
	movi	r1, SEND		// r1 = SEND (call_nr)
	movi	r0, IPCVEC_INTR		// r0 = IPC vector
	syscall
	mov	r15, r14

// _ipc_receive_intr(src, msg_ptr, status_ptr)
// r0=src, r1=msg_ptr, r2=status_ptr
ENTRY(_ipc_receive_intr)
	sw	r2, sp, -4		// save status_ptr
	subi	sp, sp, 4
	mov	r3, r1			// r3 = msg_ptr
	mov	r2, r0			// r2 = src (src_dst_e)
	movi	r1, RECEIVE		// r1 = RECEIVE
	movi	r0, IPCVEC_INTR
	syscall
	// r0 = return value, r1 = IPC status
	lw	r2, sp, 0		// restore status_ptr
	addi	sp, sp, 4
	sw	r1, r2, 0		// *status_ptr = r1
	mov	r15, r14

// _ipc_sendrec_intr(srcdest, msg_ptr)
ENTRY(_ipc_sendrec_intr)
	mov	r3, r1			// r3 = msg_ptr
	mov	r2, r0			// r2 = srcdest
	movi	r1, SENDREC
	movi	r0, IPCVEC_INTR
	syscall
	mov	r15, r14

// _ipc_notify_intr(dst)
ENTRY(_ipc_notify_intr)
	mov	r2, r0			// r2 = dst
	movi	r1, NOTIFY
	movi	r0, IPCVEC_INTR
	syscall
	mov	r15, r14

// _ipc_sendnb_intr(dest, msg_ptr)
ENTRY(_ipc_sendnb_intr)
	mov	r3, r1			// r3 = msg_ptr
	mov	r2, r0			// r2 = dest
	movi	r1, SENDNB
	movi	r0, IPCVEC_INTR
	syscall
	mov	r15, r14

// _ipc_senda_intr(table, count)
ENTRY(_ipc_senda_intr)
	mov	r3, r0			// r3 = table
	mov	r2, r1			// r2 = count
	movi	r1, SENDA
	movi	r0, IPCVEC_INTR
	syscall
	mov	r15, r14
