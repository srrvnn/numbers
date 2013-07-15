#include <iostream>
#include <cstdlib>
#include "character.h"

using namespace std;

Character::Character(unsigned char (*myimage)[256], int ulr, int ulc, int h, int w) {

	sr = ulr;
	sc = ulc;

	height = h; 
	width = w; 

	aspectratio = (float)  height / width; 

	image = myimage;

	bq[0] = bq[1] = bq[2] = bq[3] = bq[4] = bq[5] = 0;
}

void Character::perform_tests() {

	compute_euler();
	compute_centroid();
	compute_symmetry();

}

void Character::classify(){

	if(euler == -1) recognized_char = (char)8;

	else if(euler == 0) {

		if(vsymmetry > 0.95 && hsymmetry > 0.95) recognized_char = (char)0;
		else if(hsymmetry > 0.78) recognized_char = (char)6;
		else if(vsymmetry > 0.7 && hsymmetry > 0.7) recognized_char = (char)4;
		else recognized_char = (char)9;
	}

	else {

		if(aspectratio > 1.8) recognized_char = (char)1;
		else if(abs((int)(hcentroid - vcentroid)) < 0.5) recognized_char = (char)7;
		else if(vsymmetry > 0.8) recognized_char = (char)3;
		else if(vsymmetry > 0.58 && hsymmetry > 0.58) recognized_char = (char)2;
		else recognized_char = (char)5;
	}

}

char Character::get_classification(){

	return recognized_char;
}

void Character::print_results(){

	// if(euler != 1) return;

	
	cout<<"Character in bounding box: "<<sr<<","<<sc<<" to "<<sr+height<<","<<width<<endl;	
	cout<<"========================================="<<endl;

	cout<<"BQ0="<<bq[0]<<" BQ1="<<bq[1]<<" BQ2="<<bq[2]<<" BQ3="<<bq[3]<<" BQ4="<<bq[4]<<" BQD="<<bq[5]<<endl;
	cout<<"Euler number: "<<euler<<endl;		
	cout<<"VCentroid="<<vcentroid<<" / HCentroid="<<hcentroid<<endl;
	cout<<"VSym="<<vsymmetry<<"/ HSym:="<<hsymmetry<<endl;
	cout<<"Aspect ratio="<<aspectratio<<endl;
	cout<<"Character is classified as:            "<<(int)recognized_char<<endl;

	cout<<"\n";

}

void Character::getQuad(int x, int y, int *r){

	int count = 0; 

	if(image[x][y] == (char)0) count++;
	if(image[x+1][y] == (char)0) count++;
	if(image[x+1][y+1] == (char)0) count++;
	if(image[x][y+1] == (char)0) count++;

	if(count == 2 && (image[x][y] == image[x+1][y+1])){
		count = 5; 
	}

	// if(count ==1 || count ==3){
	// 	cout<<"("<<x<<","<<y<<"):"<<count<<endl;	
	// }
	

	*r = count; 
}

void Character::compute_euler(){

	euler = 0; 
	int category = 0; 

	for(int i = sr-1; i <= sr + height + 1; i++){
		for(int j = sc-1; j <= sc + width + 1; j++){
			
			getQuad(i, j, &category);

			bq[category]++;

			// if(category == 0) bq[0]++
			// else if(category == 1) bq[1]++;
			// else if(category == 2) bq[2]++;
			// else if(category == 3) bq[3]++;
			// else if(category == 4) bq[4]++;
			// else if(category == 5) bq[5]++;
		}
	}

	euler = (bq[1] - bq[3] - (2 * bq[5])) / 4; 	

	// cout<<"Q1: "<<Q1<<", Q3: "<<Q3<<", Qd: "<<Qd<<" Euler: "<<euler<<endl;	
}

void Character::compute_centroid(){

	
	int i,j;
	int vnumerator = 0, hnumerator = 0, denominator = 0;

	for( i = sr; i < sr + height; i++){
		for( j = sc; j < sc + width; j++){

			if(image[i][j] == (char)0){

				vnumerator += i-sr;
				hnumerator += j-sc;
				denominator++;
			}
		}
	}

	vcentroid = (float) vnumerator / denominator;
	hcentroid = (float) hnumerator / denominator;
}

void Character::compute_symmetry(){

	int sx = sr, sy = sc, ex = sr + height, ey = sc + width; 
	int vsymcount = 0, hsymcount = 0, vpixelcount = 0, hpixelcount = 0; 

	while(sx <= ex){

		for(int t = sy; t < sy + width; t++){

			if(image[sx][t] == image[ex][t]) vsymcount++;
			vpixelcount++;
		}	

		sx++; ex--;			
	}

	while(sy <= ey){

		for(int t = sx; t < sx + width; t++){

			if(image[t][sy] == image[t][ey]) hsymcount++;
			hpixelcount++;
		}	

		sy++; ey--;	
	}

	vsymmetry = (float) vsymcount / vpixelcount;
	hsymmetry = (float) hsymcount / hpixelcount;

}