#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int sum = 0;
    float average = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sum = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            average = (float) sum / 3;

            image[i][j].rgbtBlue = (int) round(average);
            image[i][j].rgbtGreen = (int) round(average);
            image[i][j].rgbtRed = (int) round(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    int count = 0;

    if (width % 2 == 0)
    {
        for (int i = 0; i < height; i++)
        {
            count = width - 1;

            for (int j = 0; j < (width / 2); j++)
            {
                tmp[i][j] = image[i][j];
                image[i][j] = image[i][count];
                image[i][count] = tmp[i][j];

                count--;
            }
        }
    }
    else
    {
        for (int i = 0; i < height; i++)
        {
            count = width - 1;

            for (int j = 0; j < (width - 1) / 2; j++)
            {
                tmp[i][j] = image[i][j];
                image[i][j] = image[i][count];
                image[i][count] = tmp[i][j];

                count--;
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }
    
    int blue_sum = 0;
    int green_sum = 0;
    int red_sum = 0;
    int count = 0;
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blue_sum = 0;
            green_sum = 0;
            red_sum = 0;
            count = 0;

            for (int l = i - 1; l <= i + 1; l++)
            {
                if (l < 0)
                {
                    l = 0;
                }
                
                if (l == height)
                {
                    break;
                }
                
                for (int w = j - 1; w <= j + 1; w++)
                {
                    if (w < 0)
                    {
                        w = 0;
                    }
                    
                    if (w == width)
                    {
                        break;
                    }
                    
                    blue_sum += tmp[l][w].rgbtBlue;
                    green_sum += tmp[l][w].rgbtGreen;
                    red_sum += tmp[l][w].rgbtRed;
                    
                    count++;
                }
            }
            
            image[i][j].rgbtBlue = (int) round((float) blue_sum / count);
            image[i][j].rgbtGreen = (int) round((float) green_sum / count);
            image[i][j].rgbtRed = (int) round((float) red_sum / count);
        }
    }
    
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }
    
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    
    int blue_sum_x = 0;
    int green_sum_x = 0;
    int red_sum_x = 0;
    int blue_sum_y = 0;
    int green_sum_y = 0;
    int red_sum_y = 0;
    
    int blue = 0;
    int green = 0;
    int red  = 0;
    
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blue_sum_x = 0;
            green_sum_x = 0;
            red_sum_x = 0;
            blue_sum_y = 0;
            green_sum_y = 0;
            red_sum_y = 0;
            blue = 0;
            green = 0;
            red = 0;

            for (int l = i - 1, n = 0; l <= i + 1; l++, n++)
            {
                if (l < 0)
                {
                    l = 0;
                    n = 1;
                }
                
                if (l == height)
                {
                    break;
                }
                
                for (int w = j - 1, m = 0; w <= j + 1; w++, m++)
                {
                    if (w < 0)
                    {
                        w = 0;
                        m = 1;
                    }
                    
                    if (w == width)
                    {
                        break;
                    }
                    
                    blue_sum_x += tmp[l][w].rgbtBlue * Gx[n][m];
                    green_sum_x += tmp[l][w].rgbtGreen * Gx[n][m];
                    red_sum_x += tmp[l][w].rgbtRed * Gx[n][m];
                    
                    blue_sum_y += tmp[l][w].rgbtBlue * Gy[n][m];
                    green_sum_y += tmp[l][w].rgbtGreen * Gy[n][m];
                    red_sum_y += tmp[l][w].rgbtRed * Gy[n][m];
                    
                }
            }
            
            blue = (int) round(sqrt(pow(blue_sum_x, 2) + pow(blue_sum_y, 2)));
            green = (int) round(sqrt(pow(green_sum_x, 2) + pow(green_sum_y, 2)));
            red = (int) round(sqrt(pow(red_sum_x, 2) + pow(red_sum_y, 2)));
            
            if (blue > 255)
            {
                blue = 255;
            }
            
            if (green > 255)
            {
                green = 255;
            }
            
            if (red > 255)
            {
                red = 255;
            }
            
            image[i][j].rgbtBlue = blue;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtRed = red;
        }
    }
    
    return;
}
