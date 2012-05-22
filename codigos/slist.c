/* Copyright (c) 2012 the authors listed at the following URL, and/or
the authors of referenced articles or incorporated external code:
http://en.literateprograms.org/Singly_linked_list_(C)?action=history&offset=20081126164854

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Singly_linked_list_(C)?oldid=15494
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node_s {
	void *data;
	struct node_s *next;
} NODE;

NODE *list_create(void *data)
{
	NODE *node;
	if(!(node=malloc(sizeof(NODE)))) return NULL;
	node->data=data;
	node->next=NULL;
	return node;
}
NODE *list_insert_after(NODE *node, void *data)
{
	NODE *newnode;
        newnode=list_create(data);
        newnode->next = node->next;
        node->next = newnode;
	return newnode;
}
NODE *list_insert_beginning(NODE *list, void *data)
{
	NODE *newnode;
        newnode=list_create(data);
        newnode->next = list;
	return newnode;
}
int list_remove(NODE *list, NODE *node)
{
	while(list->next && list->next!=node) list=list->next;
	if(list->next) {
		list->next=node->next;
		free(node);
		return 0;
	} else return -1;
}
int list_foreach(NODE *node, int(*func)(void*))
{
	while(node) {
		if(func(node->data)!=0) return -1;
		node=node->next;
	}
	return 0;
}
NODE *list_find(NODE *node, int(*func)(void*,void*), void *data)
{
	while(node) {
		if(func(node->data, data)>0) return node;
		node=node->next;
	}
	return NULL;
}
int printMaquinas(void *m)
{
    int i;
    MAQUINAS *maquina;
    maquina = (MAQUINAS *)m;
	printf(" - MAC= %02X", maquina->mac[0]);
    for(i=1; i<6; i++){
        printf(":%02X", maquina->mac[i]);
    }/*
                    printf("\n");
                    printf("  MAC Destination: ");

                    for(i=0; i<6; i++){
                        printf("%02X:", etherhdr->ether_dhost[i]);
                    }
                    printf("\n             Type: %04X\n", ntohs(etherhdr->ether_type));


                    printf("IP\n");
                    
                    memcpy(&source, &iphdrptr->saddr, 4);
                    memcpy(&dest, &iphdrptr->daddr, 4);
                    printf("        IP Source: %s\n", inet_ntoa(source));
                    printf("   IP Destination: %s\n", inet_ntoa(dest));

                    printf("ICMP\n");
	
	
	printf("%s\n", (char *)m);*/
	return 0;
}
int findstring(void *listdata, void *searchdata)
{
	return strcmp((char*)listdata, (char*)searchdata)?0:1;
}

/*
int main()
{
	NODE *list, *second, *inserted;
	NODE *match;

	// Create initial elements of list
	list=list_create((void*)"First");
	second=list_insert_after(list, (void*)"Second");
	list_insert_after(second, (void*)"Third");

	printf("Initial list:\n");
	list_foreach(list, printstring);
	putchar('\n');

	// Insert 1 extra element in front
	list=list_insert_beginning(list, "BeforeFirst");
	printf("After list_insert_beginning():\n");
	list_foreach(list, printstring);
	putchar('\n');

	// Insert 1 extra element after second
	inserted=list_insert_after(second, "AfterSecond");
	printf("After list_insert_after():\n");
	list_foreach(list, printstring);
	putchar('\n');

	// Remove the element
	list_remove(list, inserted);
	printf("After list_remove():\n");
	list_foreach(list, printstring);
	putchar('\n');

	// Search
	if((match=list_find(list, findstring, "Third")))
		printf("Found \"Third\"\n");
	else printf("Did not find \"Third\"\n");

	return 0;
}
*/
