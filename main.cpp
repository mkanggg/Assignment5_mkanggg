#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include "pixel.h"



const int Y_dimension = 256;     // Vertical height of 256 pixels (y ranges from 0 - 255)


 
// Calculates and prints mean of R,B,G across all pixels
void average_colors(std::vector<Pixel> &pixel_list)
{
    if(pixel_list.empty())    // Avoid divide with zero
    {
        std::cout << "Pixel list empty\n";
        return;
    }
    
    // Color sums
    double sum_r = 0.0;
    double sum_g = 0.0;
    double sum_b = 0.0;
    
    for(const auto &p : pixel_list)   // Loop to add up the colors for every pixel
    {
        sum_r += p.r;
        sum_g += p.g;
        sum_b += p.b;
    }
    
    // Divide color by numbers of pixels to get the average
    size_t count = pixel_list.size();
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Average R: " << (sum_r / count) << "\n";
    std::cout << "Average G: " << (sum_g / count) << "\n";
    std::cout << "Average B: " << (sum_b / count) << "\n";
}


// Turns picture upside down
void flip_vertically(std::vector<Pixel> &pixel_list)
{
    const int max_y_index = Y_dimension - 1;    // let the max_y_index = (last row)

    for (auto &p : pixel_list)   // Loop to flip y position ( row 0-255 into 255-0)
    {
        p.y = max_y_index - p.y;
    }
    
    std::sort(pixel_list.begin(),pixel_list.end(), [](const Pixel &a, const Pixel &b)   // Help sort pixels back to normal order
    {
        if (a.x != b.x) return a.x < b.x;
	return a.y < b.y;
    });
}



int main(int argc, char* argv[])
{
   if(argc < 2)  // Make sure the user plug in a file name
   {
       std::cerr << "usage" << argv[0] << "<input_file_path>\n";  // Print out give the user warning to put a file name 
       return 1;
   }

   // Open file given by the user
   std::string filename = argv[1];
   std::ifstream infile(filename);


   // Stop if file cannot open
   if(!infile.is_open())
   {
       std::cerr << "Error" << filename << "\n";
       return 1;
   }
   
   
   // List to hold all pixels
   std::vector<Pixel> pixel_list;
   pixel_list.reserve(131072);  // Space so its run fast


   std::string line;
   while(std::getline(infile, line))   // Read file line by line
   {
       if(line.empty()) continue;   // If theres a blank line --> Skip
       size_t start = 0;
       size_t comma_pos = 0;
       std::string tokens[5];   // Hold 5 values x,y,r,g,b
       int token_idx = 0;
	
       while(token_idx < 4 && (comma_pos = line.find(',', start)) != std::string::npos)   // Cut lines into  pieces between commas
       {
           tokens[token_idx++] = line.substr(start,comma_pos - start);
	   start = comma_pos + 1;   // if theres a comma, move past it
       }
       tokens[token_idx] = line.substr(start);   // grab the last number


   // Turn text into a number
   Pixel p;
   p.x = std::stoi(tokens[0]);
   p.y = std::stoi(tokens[1]);
   p.r = std::stof(tokens[2]);
   p.g = std::stof(tokens[3]);
   p.b = std::stof(tokens[4]);


   // Add this pixel into big list
   pixel_list.push_back(p);
   }
  
   // Close input file when done reading
   infile.close();

   std::cout << "Loaded " << pixel_list.size() << " pixels successfully.\n";

   average_colors(pixel_list);
   flip_vertically(pixel_list);

 
   // Create the output file
   std::ofstream outfile("flipped.dat");
   if(!outfile.is_open())
   {
       std::cerr << "Error: Could not open flipped.dat for writing.\n";
       return 1;

   }


   // Write pixels back to new file with clean decimal numbers
   outfile << std::setprecision(16);
   for (const auto &p : pixel_list)    // Loop to write all pixels into the new file
   {
       outfile << p.x << ","
	       << p.y << ","
	       << p.r << ","
	       << p.g << ","
	       << p.b << "\n";
   }
   outfile.close();   // Close output file

   std::cout << "Flipped pixel data written to flipped.dat\n";
   
   return 0;

}

