#ifndef CHARACTER_H_
#define CHARACTER_H_

class Character {

private: 

	// data members

	int sr, sc; 
	int height, width;

	int euler; 
	int bq[6];
	float aspectratio;
	float vcentroid; 
	float hcentroid;  

	float vsymmetry;
	float hsymmetry;

	char recognized_char;
	unsigned char (*image)[256];

	// member functions

	void compute_euler();
	void compute_centroid();
	void compute_symmetry();

	void getQuad(int, int, int*);

public: 

	// member functions

	Character(unsigned char (*myimage)[256], int ulr, int ulc, int h, int w);

	void print_boundingbox();

	void perform_tests();
	void classify();

	char get_classification();
	
	void print_results();

	int getEuler();

	~Character() {};

};

#endif // CHARACTER_H_
