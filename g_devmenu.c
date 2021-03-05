
#include "g_local.h"

#ifdef LOX	// LOX uses old 3.20 PMenu code. (see below)

/* Development menu */
// This is the maximum size the selection menu can take.
// 18 lines by 28 characters. Align left and right lose 2 chars for margins.
// Note: line index starts at zero.

pmenu_t testmenu[] = 
{
	{ "*Lethal Orifice eXamination:",	PMENU_ALIGN_CENTER, NULL, NULL },
	{ "1234567890123456789012345678",	PMENU_ALIGN_CENTER, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL }, 
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL, NULL },
	{ "1234567890123456789012345678",	PMENU_ALIGN_CENTER, NULL, NULL }
};

// Development menu command
// Prototype in g_cmds.c:
// void Cmd_DevMenu_f(char *cmd, edict_t *ent);


// Interface in g_local.h gclient_s:
// 	pmenu_t		*devmenu;		// this is a pointer to the menu
// to be assigned in PMenu_Open

// Call in ClientCommand: 
//		if (Q_stricmp (cmd, "devmenu") == 0)
//			Cmd_DevMenu_f(cmd, ent);

// Command format:
// devmenu open | close | reset
// devmenu text "string" line_number justification (l|c|r)
// * at the start of the string makes highlighted text.
// Defaults to left justify.
//


void Cmd_DevMenu_f(char *scmd, edict_t *ent)
{
	int i;
	char *p;
	
	// the command is passed in full (devmenu xxx "xxx" l j)
	scmd = gi.argv(1); // the second level command
	
	if (Q_stricmp (scmd, "open") == 0)
	{
		// allocate some space for the dynamic menu
		ent->client->devmenu = testmenu;
		PMenu_Open (ent, ent->client->devmenu, 0, sizeof(testmenu) / sizeof(pmenu_t));
	}
	
	else if (Q_stricmp (scmd, "close") == 0)
	{
		PMenu_Close(ent);
	}
	
	else if (Q_stricmp (scmd, "text") == 0)
	{
		if (gi.argc() == 4 || gi.argc() == 5)
		{
			i = atoi(gi.argv(3));
			if ( 0 > i || i > 17) // range check line number
			{
				gi.cprintf (ent, PRINT_HIGH, "Line number must be between 0 and 17.\n");
				return;
			}
			p = testmenu[i].text;
			if (strlen(gi.argv(2)) < 29) // 28 chars plus '\0' per line
				strcpy(p, gi.argv(2)); // copy at most 28 chars to the text location
			else
			{
				gi.cprintf (ent, PRINT_HIGH, "Number of characters must be less than 29.\n");
				return;
			}
			
			p = gi.argv(4);
			if (*p == 'c')
				testmenu[i].align = PMENU_ALIGN_CENTER;
			else if (*p == 'r')
				testmenu[i].align = PMENU_ALIGN_RIGHT;
			else
				testmenu[i].align = PMENU_ALIGN_LEFT;
			
			testmenu[i].arg = NULL;
			testmenu[i].SelectFunc = NULL;
		}
		else
			gi.cprintf (ent, PRINT_HIGH, "Number and text must be supplied (ex: devmenu text \"hello\" 3 l|c|r)\n");
	}
	
	else if (Q_stricmp (scmd, "reset") == 0)
	{
		for (i=0; i < (sizeof(testmenu) / sizeof(pmenu_t)); i++) 
		{
			strcpy(testmenu[i].text,""); // blank the line
			testmenu[i].align = PMENU_ALIGN_CENTER;
			testmenu[i].arg = NULL;
			testmenu[i].SelectFunc = NULL;
		}
		gi.cprintf (ent, PRINT_HIGH, "Menu reset\n");
	}
}

#else		

// Use this version if you use the PMenu code from the 3.21 release.
// This version uses the testmenu as a template to fill the
// dynamically allocated menu. PMenuOpen creates a handle
// to the new menu and alterations occur there. The menu MUST
// be opened before you can change it.
// Call: Cmd_DevMenu_f(cmd, ent);
//
pmenu_t testmenu[] = {
	{ "*Lethal Orifice eXamination:",	PMENU_ALIGN_CENTER, NULL },
	{ "1234567890123456789012345678",	PMENU_ALIGN_CENTER, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL }, 
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "                            ",	PMENU_ALIGN_LEFT, NULL },
	{ "1234567890123456789012345678",	PMENU_ALIGN_CENTER, NULL }
};

void Cmd_DevMenu_f(char *scmd, edict_t *ent)
{
	int i, align;
	char *p, *text;
	pmenuhnd_t *hnd;
	
	// the command is passed in full (devmenu xxx "xxx" l j)
	scmd = gi.argv(1); // ptr to the second level command
	
	if (Q_stricmp (scmd, "open") == 0)
	{
		hnd = PMenu_Open (ent, testmenu, 0, sizeof(testmenu) / sizeof(pmenu_t), NULL);
	}
	
	else if (Q_stricmp (scmd, "close") == 0)
	{
		PMenu_CloseIt(ent);
	}
	
	else if (Q_stricmp (scmd, "text") == 0)
	{
		if (ent->client->menu)	// menu must be open
		{
			if (gi.argc() == 4 || gi.argc() == 5)
			{
				i = atoi(gi.argv(3));
				if ( 0 > i || i > 17) // range check line number
				{
					gi.cprintf (ent, PRINT_HIGH, "Line number must be between 0 and 17.\n");
					return;
				}
				p = testmenu[i].text;
				if (strlen(gi.argv(2)) < 29) // 28 chars plus '\0' per line
					text = G_CopyString(gi.argv(2)); // copy at most 28 chars to the text location
				else
				{
					gi.cprintf (ent, PRINT_HIGH, "Number of characters must be less than 29.\n");
					return;
				}
				
				p = gi.argv(4);
				if (*p == 'c')
					align = PMENU_ALIGN_CENTER;
				else if (*p == 'r')
					align = PMENU_ALIGN_RIGHT;
				else
					align = PMENU_ALIGN_LEFT;
				
				hnd = ent->client->menu; // pointer to the dynamic menu
				hnd->entries[i].text = G_CopyString(text);	// dynamically allocated string
				hnd->entries[i].align = align;
			}
			else
				gi.cprintf (ent, PRINT_HIGH, "Number and text must be supplied (ex: devmenu text \"hello\" 3 l|c|r)\n");
		}
		else
			gi.cprintf (ent, PRINT_HIGH, "Menu must be open first.\n");
	}
	
	else if (Q_stricmp (scmd, "reset") == 0)
	{
		if (ent->client->menu)	// menu must be open
		{
			hnd = ent->client->menu; // point to the dynamic menu
			for (i=0; i < (sizeof(testmenu) / sizeof(pmenu_t)); i++) 
			{
				hnd->entries[i].text = G_CopyString(""); // blank the line
				hnd->entries[i].align = PMENU_ALIGN_CENTER;
				hnd->entries[i].SelectFunc = NULL;
				
			}
			gi.cprintf (ent, PRINT_HIGH, "Menu reset\n");
		}
	}
}

#endif
