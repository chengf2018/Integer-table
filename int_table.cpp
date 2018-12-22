#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

/*
	解析纯数字表
	作者:chengf
	日期:2018-12-22
*/

typedef struct item {
	struct item* next;
	struct item* child;
	int type;//0 nil 1 table 2 int
	int intvalue;
}ITEM;

ITEM *CreateItem();
void DeleteItem(ITEM* item);
const char *parsearray(ITEM *item, const char* value);
const char *parsenumber(ITEM *item, const char* value);
ITEM *parse(const char* value);

const char* skip(const char *str);


ITEM *CreateItem() {
	ITEM *item = (ITEM*)malloc(sizeof(ITEM));
	memset(item, 0, sizeof(ITEM));
	return item;
}

void DeleteItem(ITEM* item) {
	if (!item) return;
	ITEM * p;
	while (item) {
		if (item->type == 1 && item->child) DeleteItem(item->child);
		p = item->next;
		free(item);
		item = p;
	}
}

//跳过所有间隔符
const char* skip(const char *str) {
	while(str && *str && ((unsigned char)*str <= 32))  str++;	 
	return str;
}

ITEM *parse(const char* value) {
	if (!value) return 0;

	ITEM *item = CreateItem();
	const char *end = 0;

	value = skip(value);

	if (*value == '['){
		end = parsearray(item, skip(value));
	} else {
		end = parsenumber(item, skip(value));
	}

	if (!end) {DeleteItem(item); return 0;}

	if (*end) {DeleteItem(item); return 0;}
	return item;
}


const char *parsearray(ITEM *item, const char* value) {
	if (!value) return 0;

	const char* p = value;
	
	if (*p != '[') { return 0; }

	p = skip(p+1);
	
	if (*p == ']') { return p + 1; }

	ITEM *child;
	item->type = 1;
	child = item->child = CreateItem();
	
	if (*p == '[') {
		p = parsearray(child, skip(p));
	} else {
		p = parsenumber(child, skip(p));
	}

	p = skip(p);
	if (!p) return 0;

	while (*p == ',') {
		ITEM *newitem = CreateItem();
		child->next = newitem;
		child = newitem;
		p++;

		if (*p == '[') {
			p = parsearray(child, skip(p));
		} else {
			p = parsenumber(child, skip(p));
		}
		p = skip(p);
		if (!p) return 0;
	}
	
	if (*p == ']') return p+1;
	return 0;
}

const char *parsenumber(ITEM *item, const char* value) {
	if (!value) return 0;

	int minus = 1, n = 0;
	const char* p = value;

	if (*p == '-') { minus = -1; p++; }

	if (!(*p >= '0' && *p <= '9')) {
		return p;
	}

	while (*p >= '0' && *p <= '9') {
		n = n * 10 + (*p++ - '0');
	}

	n = minus * n;
	item->intvalue = n;
	item->type = 2;
	return p;
}

void dump_item(ITEM *item) {
	if (item) {
		ITEM *p = item;
		while (p) {
			if (p->type == 2) {
				printf("%d\t", p->intvalue);
			} else if (p->type == 1 && p->child) {
				printf("\n");
				dump_item(p->child);
			} else {
				printf("nil\t");
			}
			p = p->next;
		}
		
	} else {
		printf("nil");
	}
}

int main(int argc, char **argv) {
	const char *test = " [ [ 123 ,123, 123,-321,456,987,],[123,[777,888,999]]]";

	ITEM *item = parse(test);

	dump_item(item);

	system("pause");
	return 0;
}