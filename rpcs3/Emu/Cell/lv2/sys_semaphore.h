#pragma once

#include "sys_sync.h"

struct sys_semaphore_attribute_t
{
	be_t<u32> protocol;
	be_t<u32> pshared;
	be_t<u64> ipc_key;
	be_t<s32> flags;
	be_t<u32> pad;

	union
	{
		char name[8];
		u64 name_u64;
	};
};

struct lv2_sema final : lv2_obj
{
	static const u32 id_base = 0x96000000;

	const u32 protocol;
	const u32 shared;
	const u64 key;
	const u64 name;
	const s32 flags;
	const s32 max;

	semaphore<> mutex;
	atomic_t<s32> val;
	std::deque<cpu_thread*> sq;

	lv2_sema(u32 protocol, u64 name, s32 max, s32 value)
		: protocol(protocol)
		, shared(0)
		, key(0)
		, flags(0)
		, name(name)
		, max(max)
		, val(value)
	{
	}
};

// Aux
class ppu_thread;

// Syscalls

error_code sys_semaphore_create(vm::ps3::ptr<u32> sem_id, vm::ps3::ptr<sys_semaphore_attribute_t> attr, s32 initial_val, s32 max_val);
error_code sys_semaphore_destroy(u32 sem_id);
error_code sys_semaphore_wait(ppu_thread& ppu, u32 sem_id, u64 timeout);
error_code sys_semaphore_trywait(u32 sem_id);
error_code sys_semaphore_post(u32 sem_id, s32 count);
error_code sys_semaphore_get_value(u32 sem_id, vm::ps3::ptr<s32> count);
