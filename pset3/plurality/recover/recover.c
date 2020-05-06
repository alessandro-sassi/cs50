#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
    //ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: /recover infile\n");
        return 1;
    }

    char *card = argv[1];

    // open input file (card)
    FILE *inptr = fopen(card, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", card);
        return 2;
    }

    //temporary storages
    typedef uint8_t  BYTE;
    BYTE block[512];
    char filename[10];
    int count = 0;

    //iterate over blocks of the sd card (512 byte blocks)
    while (fread(&block, 1, 512, inptr) == 512)
    {

        //check if the header of the block is a jpeg
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0)
        {

            //create a file.jpg and store it in an array called filename, and keep track of the num. of the jpg
            sprintf(filename, "%03i.jpg", count);
            count++;

            // open filename for writing and write the jpeg header into it
            FILE *img = fopen(filename, "w");

            if (img == NULL)
            {
                fprintf(stderr, "Could not open %s.\n", filename);
                return 12;
            }

            fwrite(block, 1, 512, img);

            //write in the jpeg until new jpeg is found, and then exit the loop
            while (1)
            {

                // when EOF or a new jpeg is detected exit the loop
                if ((fread(&block, 1, 512, inptr) != 512) || (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0))
                {
                    fseek(inptr, -512, SEEK_CUR);
                    break;
                }

                // write a block to the jpeg
                fwrite(block, 1, 512, img);

            }

            //close the jpeg
            fclose(img);
        }
    }

    //close any remaining files
    fclose(inptr);
}
