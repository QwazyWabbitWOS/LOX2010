/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef SCANNER_H
#define SCANNER_H

// scanner consts & macros
#define	SCANNER_UNIT                   32
#define	SCANNER_RANGE                  100
#define	SCANNER_UPDATE_FREQ            1
#define	PIC_SCANNER                    "pics/scanner/scanner.pcx"
#define	PIC_SCANNER_TAG                "scanner/scanner"
#define	PIC_DOT                        "pics/scanner/dot.pcx"
#define	PIC_DOT_TAG                    "scanner/dot"
#define PIC_FARDOT					   "pics/scanner/fardot.pcx"
#define PIC_FARDOT_TAG				   "scanner/fardot"
#define PIC_FARTHERDOT				   "pics/scanner/fartherdot.pcx"
#define PIC_FARTHERDOT_TAG			   "scanner/fartherdot"
#define PIC_FARTHESTDOT				   "pics/scanner/farthestdot.pcx"
#define PIC_FARTHESTDOT_TAG			   "scanner/farthestdot"
#define	PIC_ACIDDOT                    "pics/scanner/aciddot.pcx"
#define	PIC_ACIDDOT_TAG                "scanner/aciddot"
#define	PIC_INVDOT                     "pics/scanner/invdot.pcx"
#define	PIC_INVDOT_TAG                 "scanner/invdot"
#define	PIC_QUADDOT                    "pics/scanner/quaddot.pcx"
#define	PIC_QUADDOT_TAG                "scanner/quaddot"
#define	PIC_DOWN                       "pics/scanner/down.pcx"
#define	PIC_DOWN_TAG                   "scanner/down"
#define	PIC_UP                         "pics/scanner/up.pcx"
#define	PIC_UP_TAG                     "scanner/up"
#define	PIC_SCANNER_ICON               "pics/scanner/scanicon.pcx"
#define	PIC_SCANNER_ICON_TAG           "scanner/scanicon"
#define	SAFE_STRCAT(org,add,maxlen)    if ((strlen(org) + strlen(add)) < maxlen)    strcat(org,add);
#define	LAYOUT_MAX_LENGTH              1400

// scanner function prototypes
void		ClearScanner(gclient_t *client);
void		Toggle_Scanner (edict_t *ent);
void		ShowScanner(edict_t *ent,char *layout);
//qboolean	Pickup_Scanner (edict_t *ent, edict_t *other);

#endif
