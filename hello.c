// SPDX-License-Identifier: UNLICENSED
/* Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided
 * that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgment:
 *    This product includes software developed by GlobalLogic.
 * 4. Neither the name of GlobalLogic nor the names of its contributors
 *    may be used to endorse or
 *    promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC
 * UKRAINE LLC BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Presniakov Artem <6200211@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_PARM_DESC(count, "Number of times to print 'Hello, world!'");
// Значення за замовчуванням
static unsigned int count = 1;
module_param(count, uint, 0444);	// Параметр типу uint

// Структура для списку
struct time_event {
	struct list_head list;
	ktime_t time;
};

// Голови списку
static LIST_HEAD(time_list);

static int __init hello_init(void)
{
	unsigned int i = 0;

	if (count == 0 || (count >= 5 && count <= 10)) {
		pr_warn("Warning.Continuing...\n", count);
	} else if (count > 10) {
		pr_err("Error.Module not loaded !\n", count);
		// Помилка завантаження модуля
		return -EINVAL;
	}

	 // Друкуємо "Hello, world!" count разів
	for (i = 0; i < count; i++) {
		// Виділяємо пам'ять для елемента
		struct time_event *event = kmalloc(sizeof(*event), GFP_KERNEL);

		if (!event) {

			pr_err("Error : Memory allocation failed !\n");
			// Помилка виділення пам'яті
			return -ENOMEM;
	}
	// Отримуємо поточний час ядра
	event->time = ktime_get();
	// Додаємо до списку
	list_add_tail(&event->list, &time_list);
	pr_info("Hello, world !\n");
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct time_event *event;

	struct list_head *pos, *q;

// Проходимо по списку
list_for_each_safe(pos, q, &time_list) {
	event = list_entry(pos, struct time_event, list);
	pr_info("Event time : % lld ns\n", ktime_to_ns(event->time));
	list_del(pos);	// Видаляємо елемент зі списку
	kfree(event);	// Звільняємо пам'ять
	}
}

module_init(hello_init);
module_exit(hello_exit);
