#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

/*
	References:
	-Reading files: Jack Wu's COP3503 Lab 3
	-AVL Tree Implementation: Yash Patel's Project 1 Gator AVL Project
*/

struct Song
{
	string artist;
	string name;
	int year;
	double danceability;

	Song* left;
	Song* right;
	int height;

	Song(string artist, string name, int year, double danceability)
	{
		this->artist = artist;
		this->name = name;
		this->year = year;
		this->danceability = danceability;
		this->left = nullptr;
		this->right = nullptr;
		height = 0;
	}
};

class AvlTree {

public:
	// constructor for AVL tree 
	Song* root;
	AvlTree() {
		this->root = nullptr;
	}

	//insert && deletion 
	Song* insert(Song* root, Song* newSong);

	//search functions 
	void searchNAME(Song* root, string name);

	//print functions

	int printLevelCount(Song* root);

	//balance factor 
	int balanceFactor(Song* root);

	//rotations
	Song* rotateRight(Song* node);
	Song* rotateLeft(Song* node);
	Song* rotateRightLeft(Song* node);
	Song* rotateLeftRight(Song* node);

	//helper functions 
	string findName(Song* root, string name);
	bool Songexists(Song* root, string name);
	Song* minFind(Song* root);
	int numSongs(Song* root);
	void searchARTIST(Song* root, string artist, bool& found);
	void searchYEAR(Song* root, int year, bool& found);
};

int AvlTree::printLevelCount(Song* root)
{
	//reference: https://www.javatpoint.com/cpp-algorithm-max-function

	if (root == nullptr) {
		return 0;
	}

	return 1 + max(printLevelCount(root->left), printLevelCount(root->right));
}

Song* AvlTree::insert(Song* root, Song* newSong)
{
	//reference: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
	if (root == nullptr)
	{
		return newSong;
	}

	if (newSong->name < root->name)
	{
		root->left = insert(root->left, newSong);
	}
	else if (newSong->name > root->name)												// if id greater than root, move right
	{
		root->right = insert(root->right, newSong);
	}
	else
	{
		return root;
	}																					// stops an id that is repeated 

	//now updating the height of the tree
	root->height = printLevelCount(root);												// the height of the tree 


	int bF = balanceFactor(root);														//returns balance factor of that node

	//rotations

	if (root->left && bF > 1 && newSong->name < root->left->name)
	{
		return rotateRight(root);
	}

	if (root->right && bF < -1 && newSong->name > root->right->name)
	{
		return rotateLeft(root);
	}


	if (root->left && bF > 1 && newSong->name > root->left->name)
	{
		return rotateLeftRight(root);
	}

	if (root->right && bF < -1 && newSong->name < root->right->name)
	{
		return rotateRightLeft(root);
	}
	return root;
}

void AvlTree::searchNAME(Song* root, string name)
{
	if (root == nullptr) {
		return;
	}
	//if roots name is equal to one being searched
	if (root->name == name) {
		cout << root->name << " by " << root->artist << " - " << root->year << endl;
		cout << endl;
	}
	//looking in the subtrees
	searchNAME(root->left, name);
	searchNAME(root->right, name);
}

void AvlTree::searchARTIST(Song* root, string artist, bool& found)
{
	if (root == nullptr) {
		return;
	}
	//if roots name is equal to one being searched
	if (root->artist == artist) {
		cout << root->name << " by " << root->artist << " - " << root->year << endl;
		found = true;
		cout << endl;
	}
	//looking in the subtrees
	searchARTIST(root->left, artist, found);
	searchARTIST(root->right, artist, found);
}

void AvlTree::searchYEAR(Song* root, int year, bool& found)
{
	if (root == nullptr) {
		return;
	}
	//if roots name is equal to one being searched
	if (root->year == year) {
		cout << root->name << " by " << root->artist << " - " << root->year << endl;
		found = true;
		cout << endl;
	}
	//looking in the subtrees
	searchYEAR(root->left, year, found);
	searchYEAR(root->right, year, found);
}

int AvlTree::balanceFactor(Song* root)
{
	if (root == nullptr) {																	// to get height of left subtree and subtract from right subtree
		return 0;
	}
	return printLevelCount(root->left) - printLevelCount(root->right);
}

Song* AvlTree::rotateRight(Song* node)
{
	//left left case taken from Stepik submission 5.1.1
	Song* gchild = node->left->right;
	Song* newP = node->left;
	newP->right = node;
	node->left = gchild;

	//update heights
	node->height = printLevelCount(node);
	newP->height = printLevelCount(newP);

	return newP;
}

Song* AvlTree::rotateLeft(Song* node)
{
	//right right case taken from Stepik submission 5.1.1
	Song* gchild = node->right->left;
	Song* newP = node->right;
	newP->left = node;
	node->right = gchild;

	//update heights
	node->height = printLevelCount(node);
	newP->height = printLevelCount(newP);

	return newP;
}

Song* AvlTree::rotateRightLeft(Song* node)
{
	//rotate right first on child
	node->right = rotateRight(node->right);
	//then rotate left on the parent node
	node = rotateLeft(node);
	return node;
}

Song* AvlTree::rotateLeftRight(Song* node)
{
	node->left = rotateLeft(node->left);
	node = rotateRight(node);
	return node;
}

string AvlTree::findName(Song* root, string name)
{
	//reference: https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/

	if (root == nullptr) {
		return "";
	}
	if (root->name == name) {
		return root->name; //if you found the NAME then you would return that string
	}
	//if the root is less than the id, then you need to move right
	//else move to the left
	if (root->name < name) {
		return findName(root->right, name);
	}
	else {
		return findName(root->left, name);
	}
}

bool AvlTree::Songexists(Song* root, string name)
{
	//reference: https://www.geeksforgeeks.org/search-a-node-in-binary-tree/
	//this function returns yes or no if node is in the tree based on name
	if (root == nullptr) {
		return false;
	}
	//if found at the root
	if (root->name == name) {
		return true;
	}
	bool left = Songexists(root->left, name);
	if (left) {
		return true;
	}
	//keep going if not in the left side
	bool right = Songexists(root->right, name);

	return right;

}

Song* AvlTree::minFind(Song* root)
{
	//find min value in a tree
	while (root->left != nullptr) {
		root = root->left;
	}
	return root;
}


int AvlTree::numSongs(Song* root) {
	if (root == nullptr) {
		return 0;
	}
	return 1 + numSongs(root->left) + numSongs(root->right);
}

void heapify(vector<Song>& b, int n, int i) {
	int largest = i; // largest as root
	int l = 2 * i + 1; // left = 2*i + 1
	int r = 2 * i + 2; // right = 2*i + 2

	// If left child larger than root
	if (l < n && b[l].danceability > b[largest].danceability)
		largest = l;

	// If right child larger than largest so far
	if (r < n && b[r].danceability > b[largest].danceability)
		largest = r;

	// If largest is not root
	if (largest != i) {
		swap(b[i], b[largest]);

		// Recursively heapify the sub tree 
		heapify(b, n, largest);
	}
}

void buildHeap(vector<Song>& b, int n)
{
	// Index of last non-leaf node
	int startIndex = (n / 2) - 1;

	//  level order traversal  from last non-leaf node and heapify each node
	for (int i = startIndex; i >= 0; i--) {
		heapify(b, n, i);
	}
}

int main()
{
	string filename = "data_edited_no_commas.csv";
	//string filename = "testing.csv";

	AvlTree b;
	vector<Song>a;

	ifstream inFile(filename);

	if (inFile.is_open()) {
		string dataHeader;
		getline(inFile, dataHeader);

		string songRow;
		string artist, name;
		double danceability;
		int year;
		//int number, pieces, minifigs;
		string sDanceability, sYear;

		while (getline(inFile, songRow)) {
			istringstream stream(songRow);

			getline(stream, artist, ',');
			getline(stream, sDanceability, ',');
			getline(stream, name, ',');
			getline(stream, sYear, ',');

			danceability = stod(sDanceability);
			year = stoi(sYear);

			Song* s = new Song(artist, name, year, danceability);
			Song d(artist, name, year, danceability);
			a.push_back(d);

			b.root = b.insert(b.root, s);
		}

	}

	cout << "--------------------" << endl;
	cout << "|                  |" << endl;
	cout << "|    Welcome to    |" << endl;
	cout << "|   Gator Nights   |" << endl;
	cout << "|                  |" << endl;
	cout << "|  Karaoke Machine |" << endl;
	cout << "|                  |" << endl;
	cout << "--------------------" << endl;
	cout << "Number of songs available: " << b.numSongs(b.root) << endl;
	int option = 1;
	string name;
	string name2;
	string artist;
	int year;
	int danceable;
	bool foundArtist = false;
	bool foundYear = false;

	while (option != 0)
	{
		cout << " Menu:" << endl;
		cout << " 0. Exit" << endl;
		cout << " 1. Search song title" << endl;
		cout << " 2. Search by artist" << endl;
		cout << " 3. Search by year" << endl;
		cout << " 4. Find most danceable song" << endl;
		cout << endl;
		cout << "What would you like to do? ";
		cin >> option;


		if (option == 0)
		{
			break;
		}
		else if (option == 1)
		{
			cout << "Enter song title: ";
			cin.ignore();
			getline(cin, name);
			cout << "Now playing: ";
			b.searchNAME(b.root, name);
		}
		else if (option == 2)
		{
			cout << "Enter artist name: ";
			cin.ignore();
			getline(cin, artist);

			cout << "Songs available by artist: " << endl;
			artist = "['" + artist + "']";

			b.searchARTIST(b.root, artist, foundArtist);

			if (foundArtist == false)
			{
				cout << "No songs available" << endl;
				cout << endl;
			}
			else
			{
				cout << "Which song would you like to play? ";
				getline(cin, name2);
				cout << "Now playing: ";
				b.searchNAME(b.root, name2);
			}
			foundArtist = false;
		}
		else if (option == 3)
		{
			cout << "Enter song year: " << endl;
			cin >> year;

			cout << "Songs released in " << year << " : " << endl;

			b.searchYEAR(b.root, year, foundYear);

			if (foundYear == false)
			{
				cout << "No songs available" << endl;
				cout << endl;
			}
			else
			{
				cout << "Which song would you like to play? ";
				cin.ignore();
				getline(cin, name2);
				cout << "Now playing: ";
				b.searchNAME(b.root, name2);
			}
			foundYear = false;
		}
		else if (option == 4)
		{
			int x = 0;
			char c;
			char b;
			cout << endl;
			cout << "Most danceable song: ";
			buildHeap(a, a.size());
			cout << a[x].name << " by " << a[x].artist << " - " << a[x].year << endl;
			cout << "Want to play this song ?" << endl;
			cout << "Enter Y or N: ";
			cin >> c;
			if (c != 'Y') {
				x++;
				cout << "Next most danceable song: ";
				cout << a[x].name << " by " << a[x].artist << " - " << a[x].year << endl;
				cout << "Want to play this song ?" << endl;
				cout << "Enter Y or N: ";
				cin >> b;
				while (b != 'Y') {
					x++;
					cout << "Next most danceable song: ";
					cout << a[x].name << " by " << a[x].artist << " - " << a[x].year << endl;
					cout << "Want to play this song ?" << endl;
					cout << "Enter Y or N: ";
					cin >> b;
				}
				cout << "Now playing: ";
				cout << a[x].name << " by " << a[x].artist << " - " << a[x].year << endl;
				cout << endl;

			}
			else {
				cout << "Now playing: ";
				cout << a[x].name << " by " << a[x].artist << " - " << a[x].year << endl;
				cout << endl;
			}
		}
	}
}
