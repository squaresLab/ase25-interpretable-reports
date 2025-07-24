//FormAI DATASET v1.0 Category: Color Code Converter ; Style: detailed
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int option;
    float r, g, b;
    char ch, color[20];
    printf("Welcome to color code converter!\n");
    printf("Enter the color model that you have:\n");
    printf("1. RGB\n2. HEX\n3. CMYK\n");
    scanf("%d",&option);
    switch(option)
    {
        case 1:
            printf("Enter the RGB values separated by a space: ");
            scanf("%f%f%f",&r, &g, &b);
            printf("The RGB color code is (%d, %d, %d)\n",(int)r,(int)g,(int)b);
            printf("The HEX color code is #%02X%02X%02X\n",(int)r,(int)g,(int)b);
            printf("The CMYK color code is ");
            float c = 1.0 - (r/255.0);
            float m = 1.0 - (g/255.0);
            float y = 1.0 - (b/255.0);
            float k = (c < m) ? c : m;
            k = (k < y) ? k : y;
            if(k == 1.0)
            {
                c = m = y = 0.0;
            }
            else
            {
                c = (c - k) / (1.0 - k);
                m = (m - k) / (1.0 - k);
                y = (y - k) / (1.0 - k);
            }
            printf("(%d%%, %d%%, %d%%, %d%%)\n",(int)(c*100),(int)(m*100),(int)(y*100),(int)(k*100));
            break;
            
        case 2:
            printf("Enter the HEX color code: ");
            scanf("%s",color);
            printf("The RGB color code is (%d, %d, %d)\n", 
                (int)strtol(color+0, &ch, 16),
                (int)strtol(color+2, &ch, 16),
                (int)strtol(color+4, &ch, 16));
            printf("The CMYK color code is ");
            r = (float)(strtol(color+0, &ch, 16)) / 255.0;
            g = (float)(strtol(color+2, &ch, 16)) / 255.0;
            b = (float)(strtol(color+4, &ch, 16)) / 255.0;
            float c1 = 1.0 - r;
            float m1 = 1.0 - g;
            float y1 = 1.0 - b;
            float k1 = (c1 < m1) ? c1 : m1;
            k1 = (k1 < y1) ? k1 : y1;
            if(k1 == 1.0)
            {
                c1 = m1 = y1 = 0.0;
            }
            else
            {
                c1 = (c1 - k1) / (1.0 - k1);
                m1 = (m1 - k1) / (1.0 - k1);
                y1 = (y1 - k1) / (1.0 - k1);
            }
            printf("(%d%%, %d%%, %d%%, %d%%)\n",(int)(c1*100),(int)(m1*100),(int)(y1*100),(int)(k1*100));
            break;
            
        case 3:
            printf("Enter the CMYK values separated by a space: ");
            scanf("%f%f%f%f",&c,&m,&y,&k);
            printf("The RGB color code is (%d, %d, %d)\n",(int)((1-c)*(1-k)*255),(int)((1-m)*(1-k)*255),(int)((1-y)*(1-k)*255));
            printf("The HEX color code is #%02X%02X%02X\n",(int)((1-c)*(1-k)*255),(int)((1-m)*(1-k)*255),(int)((1-y)*(1-k)*255));
            printf("The CMYK color code is (%d%%, %d%%, %d%%, %d%%)\n",(int)(c*100),(int)(m*100),(int)(y*100),(int)(k*100));
            break;
            
        default:
            printf("Invalid input. Please try again.\n");
    }
    return 0;
}