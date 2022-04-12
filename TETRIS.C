/* TETRIS by Mike Ward
   Link with PCIO
   V1.0 Dec 1993
*/

#include "stdio.h"

extern char scr_attr;

int a,b,c,ky;
int score,x,y,ch,sh,rt,nch,nsh,nrt,col,nco;
int bag[7],bagidx;
int lines,speed,level;

char x2[10];
char dt[28][4][5]={ "0100", "0100", "0100", "0100", "0000", "0000", "1111",
"0000", "0100", "0100", "0100", "0100", "0000", "0000", "1111", "0000",
"0100", "0100", "0110", "0000", "0000", "0111", "0100", "0000", "0000",
"0110", "0010", "0010", "0000", "0010", "1110", "0000", "0010", "0010",
"0110", "0000", "0000", "0100", "0111", "0000", "0000", "0110", "0100",
"0100", "0000", "1110", "0010", "0000", "0000", "1110", "0100", "0000",
"0000", "0010", "0110", "0010", "0000", "0000", "0100", "1110", "0000",
"1000", "1100", "1000", "0100", "0110", "0010", "0000", "0000", "0011",
"0110", "0000", "0100", "0110", "0010", "0000", "0000", "0011", "0110",
"0000", "0010", "0110", "0100", "0000", "0000", "0110", "0011", "0000",
"0010", "0110", "0100", "0000", "0000", "0110", "0011", "0000", "0000",
"0110", "0110", "0000", "0000", "0110", "0110", "0000", "0000", "0110",
"0110", "0000", "0000", "0110", "0110", "0000" };

main()
{
scr_setup();
scr_setmode(1);
scr_cursoff();
scr_rowcol(0,8);
scr_aputs("T E T R I S  by M. Brent",14);
scr_attr=127;
for (a=1; a<25; a++)
{ scr_rowcol(a,14);
  scr_co(219);
  scr_rowcol(a,25);
  scr_co(219);
}
for (a=15; a<25; a++)
{ scr_rowcol(24,a);
  scr_co(219);
} 
scr_rowcol(12,1);
scr_aputs("Score:",9);
scr_attr=43;
scr_rowcol(12,8);
printf("%5d",score);
scr_rowcol(12,29);
scr_aputs("NEXT",112);
scr_rowcol(14,26);
puts("A - left");  
scr_rowcol(15,26);
puts("S - right");
scr_rowcol(16,26);
puts("<enter> - spin");
scr_rowcol(17,26);
puts("<space> - drop");
scr_rowcol(18,26);
puts("P - pause");
scr_rowcol(19,26);
puts("<esc> - quit");
while (ky!=27)
{ scr_rowcol(5,15);
  scr_aputs("Game Over",132);
  while (scr_csts());
  ky=0;
  while (ky==0)
  { ky=scr_csts(); rand(); }
  clw();
  if (ky!=27)
    game();
}  
  scr_setmode(2);
  sound_off();
printf("\nThis particular clone of Tetris is (c)1993\n");
printf("by Mike Brent. If you like it, let me know!\n\n");
printf("M.Brent\n");
printf("PO Box 42\n");
printf("Hedley, BC, Canada\n");
printf("V0X 1K0\n\n");
printf("EMail: mbrent@awinc.com\n");
printf("WWW: http:\\\\www.awinc.com\\users\\mbrent\n");

}

draw(x,y,s,r,a,c) int x,y,s,r,a; char c;
{ /* draw at x,y, shape s, rotation r, character c */
int q,w;
char qw[2];

qw[0]=c; qw[1]=0;
for (q=0; q<4; q++)
 for (w=0; w<4; w++)
  if (dt[s*4+r][q][w]=='1') {scr_rowcol(y+q,x+w); scr_aputs(qw,a);}
}

clw()
{ /* clear window */
int q;

for (q=1; q<24; q++)
{ scr_rowcol(q,15);
  scr_aputs("          ",7);
}
}

check(x,y,s,r) int x,y,s,r;
{ /* check if shape is legal at x,y, shape s, rotation r */
  /* return 1 if illegal */
int q,w,f;

f=0;
for (q=0; q<4; q++)
 for (w=0; w<4; w++)
 { scr_rowcol(y+q,x+w);
   if ((scr_sinp()!=32)&&(dt[s*4+r][q][w]=='1')) f=1;
 }
return(f);
}

set_speed()
{ /* set the variable "speed" based on the machine */
char x1[10],x2[10];
int q;

q=0;
times(x1);
times(x2);
while (x2[7]==x1[7])
 times(x1);
times(x2);
while (x2[7]==x1[7])
{ q++; times(x2); }
speed=q/60;
}

next()
{ /* choose next shape */
int a,b,c,d;

for (a=7; a<11; a++)
{ scr_rowcol(a,29);
  scr_aputs("    ",7);
}
ch=nch; sh=nsh; rt=nrt; col=nco;
nco=0;
while ((nco==0)||(nco==8)||((nco&7)==0))
  nco=rand()%128;
nch=rand()%4;
switch (nch)
{ case 0: nch=176; break;
  case 1: nch=177; break;
  case 2: nch=178; break;
  case 3: nch=219; break;
}

/* nsh=rand()%7; */
/* real tetris selects the 7 shapes from a bag, so */
/* you ALWAYS get one of each before a repeat. */
/* this prevents a game from going on forever. */
/* I learned this in 2016!! ;) */
/* TODO: not tested yet, I kind of need to port this */
/* to something I can still run. TI port? Could clean it up. */
if (bagidx > 0) {
  nsh = bag[--bagidx];
} else {
  // create a new bag
  for (a=0; a<7; a++) {
    bag[a]=a;
  }
  // now mix the bag up
  for (a=0; a<100; a++) {
    b=rand()%7;
    c=rand()%7;
    d=bag[b];
    bag[b]=bag[a];
    bag[a]=d;
  }
  // and choose the last value
  bagidx=7;
  nsh = bag[--bagidx];
}

nrt=rand()%4;
draw(29,7,nsh,nrt,nco,nch);
}

game()
{ /* play one game */
int z;

score=0;
bagidx = 0;
set_speed();
lines=5;
level=1;
if (ky=='1') level=10;
if (ky=='2') level=20;
scr_rowcol(15,1);
printf("Level: %2d",level);
nsh=6; nrt=0;
next();
while (1)
{ next();
  x=19; y=1;
  z=check(x,y,sh,rt);
  if ((z)||(ky==27)) return(0);
  drop();
  sound_on(11000);
  score++;
  scr_rowcol(12,8);
  printf("%5d",score);
  if (level>20) rndbl();
  delay(speed);
  sound_off();
  checkline();
  if (lines==0) newlevel();
}
}

drop()
{ /* drop current piece with key presses */
int cn,fl,df;

cn=0; fl=1; df=0;
while (fl)
{ draw(x,y,sh,rt,7,32);
  ky=scr_csts();
  if (ky!=0) df=0;
  if (ky>'Z') ky=ky-32;
  if (ky=='P')
  { /* pause */
    ky=0;
    scr_rowcol(21,26);
    scr_aputs("PAUSE",132);
    while (ky==0)
      ky=scr_csts();
    scr_rowcol(21,26);
    puts("     ");
  }
  if (ky==27) return(0);
  if (ky=='A')
   if (check(x-1,y,sh,rt)==0) x--;
  if (ky=='S')
   if (check(x+1,y,sh,rt)==0) x++;
  if (ky==' ')
  { cn=4; df=1; }
  if (ky==13)
  { rt++;
    if (rt==4) rt=0;
    if (check(x,y,sh,rt)==1)
    { rt--;
      if (rt==-1) rt=3;
    }
  }
  cn++; 
  if (cn==5) 
  { cn=0; 
    fl=1-check(x,y+1,sh,rt);
    if (fl) y++;
  }
  draw(x,y,sh,rt,col,ch);
  if (df==0) delay(speed*5);
  else delay(speed);
}}

delay(x) int x;
{ /* pauses */
int a;

for (a=0; a<x; a++)
 times(x2);
}

checkline()
{ /* check and erase lines */
int l,c,fl,x,y,as=10;
char qw[2];

l=23;

while (l>2)
{ fl=1;
  for (c=15; c<25; c++)
  { scr_rowcol(l,c);
    if (scr_sinp()==32) fl=0;
  }
  if (fl)
  { for (c=15; c<25; c++)
    { scr_rowcol(l,c);
      scr_co(32);
      sound_on((35000+(100*as))-(c*500));
      delay(speed);
    }
    sound_off();
    for (x=l-1; x>1; x--)
      for (c=15; c<25; c++)
      { scr_rowcol(x,c);
        y=scr_sinp();
        scr_rowcol(x+1,c);
        qw[0]=y; qw[1]=0;
        if (y==32) scr_aputs(qw,7);
        else scr_co(y);
      }
    score=score+as; as=as*2;
    scr_rowcol(12,8);
    printf("%5d",score);
    if (lines) {lines--; scr_rowcol(16,1); printf("Lines: %2d",lines); }
  }
    else l--;
}
}

newlevel()
{ /* set up new level */
int a,c,fl;

level++;
scr_rowcol(15,1);
printf("Level: %2d",level);
lines=level*2+5;
scr_rowcol(16,1);
printf("Lines: %2d",lines);
if (speed>3) speed=speed-2;
a=1; fl=1;
while (fl)
{ for (c=15; c<25; c++)
  { scr_rowcol(a,c);
    if (scr_sinp()!=32) fl=0;
    scr_co(176);
    sound_on(9000-a*300);
    delay(speed);
  }
  score=score+5;
  scr_rowcol(12,8);
  printf("%5d",score);
  delay(speed*2);
  a++;
}
sound_off();
delay(speed*60);
clw();
if (level>10)
{ /* handicap */
  a=(level-10)*2;
  if (a>10) a=10;
  for (a=24-a; a<24; a++)
  { c=rand()%10+15;
    scr_rowcol(a,c);
    scr_co(219);
  }
}
scr_rowcol(2,0);
switch (level)
{ case 11: puts("Handicap!"); break;
  case 21: puts("Beware!"); break;
}
if ((level==11)||(level==21)) delay(speed*200);
scr_rowcol(2,0);
puts("         ");
}

rndbl()
{ /* maybe add a random block */
int y,c,qw;

if (rand()%100<=level)
{ c=rand()%10+15;
  y=0;
  qw=32;
  while (qw==32)
  { y++; scr_rowcol(y,c);
    qw=scr_sinp();
  }
  y--;
  scr_rowcol(y,c);
  scr_co(178);
}
}

sound_on(freq) int freq;
{ _outb(182,67);
  _outb(freq%256,66);
  _outb(freq/256,66);
  _outb(_inb(97)|3,97);
}

sound_off()
{ _outb(_inb(97)&252,97);
}
