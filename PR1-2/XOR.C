/* programa XOR.C */
/* by David Corcuera Portol‚s i Joanmi Bardera Bosch */
/* 3er INFO-A, grup 3 de pr…ctiques */


#include <dos.h>
#include <stdio.h>

unsigned int Pixeladdr(x1,y1)           /* funci¢ que calcula el despla‡a- */
unsigned int x1,y1;                     /* ment de la posici¢ del pixel */
{
    asm mov bx,y1                       /* carrega y1 a bx */
    asm mov ax,x1                       /* carrega x1 a ax */

    asm xchg bh,bl                      /* intercanvia bh i bl, ja que es */
                                        /* el mateix que despla‡ar-ho 8 cops */
                                        /* donat que tenim 256 colors, i 2^8 */
                                        /* ‚s 256 = 1 byte per pixel */

    asm add ax,bx                       /* sumem bx a ax per afegir el des- */
                                        /* pla‡ament de la x */

    asm shr bx,1                        /* desplacem 2 */
    asm shr bx,1                        /* cops a la dreta bx */

    asm add ax,bx                       /* sumem bx a ax */
    asm mov word ptr [bp+6],ax          /* posem a y1 el valor del despla- */
                                        /* ‡ament. */
    return(y1);                         /* retornem el valor */
}

void SetPixel(x,y,n,modo)  /* encŠn 1 pixel a la pantalla */
unsigned int x,y;
unsigned int n,modo;
{
unsigned int VideoBufferSeg;
VideoBufferSeg=0x0a000;  /* adre‡a inici VGA */

    Pixeladdr(x,y);        /* calculem el despla‡ament del pixel */

    asm mov bx,VideoBufferSeg
    asm mov es,bx         /* direccionem la VGA amb l'extra segment */

    asm mov cx,n          /* posem el color a cx */

    asm mov bx,ax         /* carreguem el despla‡ament del pixel a bx */
                          /* perque la instruccio MOV nom‚s ens permet */
                          /* el direccionament amb bx, no amb ax */
                          /* A ax tenim el valor retornat per la funci¢ */
                          /* PixelAddr */

    if (modo==0) asm mov es:[bx],cl;      /* idem que al and.c */
        else asm xor es:[bx],cl;


}

void main(argc,argv)
char *argv[];
{
unsigned int x01,y01,x11,y11,color1,x02,y02,x12,y12,color2,j,i;
union REGS ent,sal;

    x01 = atoi(argv[1]);
    y01 = atoi(argv[2]);
    x11 = atoi(argv[3]);
    y11 = atoi(argv[4]);
    color1 = atoi(argv[5]);
    x02 = atoi(argv[6]);
    y02 = atoi(argv[7]);
    x12 = atoi(argv[8]);
    y12 = atoi(argv[9]);
    color2 = atoi(argv[10]);
    
    ent.h.al=0x013;
    ent.h.ah=0;
    int86(0x10,&ent,&sal);
    for(i=x01;i<=x11;i++)                 /* bucle per encendre tots els pixels */
    {                                     /* del quadrat, ‚s m‚s rapid si es fa */
        for(j=y01;j<=y11;j++)             /* en assembler omplint les posicions */
        SetPixel(i,j,color1,0);           /* per• no hi hauria codi C :) */
    }
    for(i=x02;i<=x12;i++)
    {
        for(j=y02;j<=y12;j++)
        if ((i>=x01) && (i<=x11) && (j>=y01) && (j<=y11))
            SetPixel(i,j,color2,3);              /* idem que al and.c */
             else SetPixel(i,j,color2,0);
    }

    i=getchar();
    ent.h.al=0x03;
    ent.h.ah=0;
    int86(0x10,&ent,&sal);          /* Tornem al mode 640x400x16 text */
}
