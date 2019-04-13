#include <iostream>
#include<time.h>
#include <stack>
#include <vector>

class SaeedMaze
{
	const int MAZE_MAX_WIDTH = 59;	//	the limit has been defined according to console width
	const int MAZE_MAX_HEIGHT = 100;
protected:

	//Maze Parameters
	int  mMazeWidth;	//	Maze Width > 2
	int  mMazeHeight;	//	Maze Height > 2
	char** mMaze;	//	contains flags to tell about the cell situation 
	std::pair<int, int> mStartPos;	//	Coordinates of the starting point
	std::pair<int, int> mEndPos;	//	Coordinates of the ending point


	// Some bit fields for convenience
	enum
	{
		CELL_PATH_N = 0x01,
		CELL_PATH_E = 0x02,
		CELL_PATH_S = 0x04,
		CELL_PATH_W = 0x08,
		CELL_VISITED = 0x10,
		CELL_START = 0x20,
		CELL_END = 0x40,
		CELL_SOLUTION = 0x80,
	};

public:
	SaeedMaze()	//	Class constructor with out inputs (randomlly chosen the size of the Maze and the coordinates of the starting/ending points)
		: mMazeWidth(2 + rand() % (MAZE_MAX_WIDTH - 1)), mMazeHeight(2 + rand() % (MAZE_MAX_HEIGHT - 1))
	{
		mStartPos = { 0 , rand() % mMazeHeight };
		mEndPos = { mMazeWidth - 1 , rand() % mMazeHeight };
		Create();
	}
	SaeedMaze(int width, int height)	//	Class constructor with two integers indicating the size of the maze (randomlly chosen the coordinates of the starting / ending points)
		: mMazeWidth(width), mMazeHeight(height)
	{
		mStartPos = { 0 , rand() % mMazeHeight };
		mEndPos = { mMazeWidth - 1 , rand() % mMazeHeight };
		Create();
	}
	SaeedMaze(int width, int height, std::pair<int, int> start, std::pair<int, int> end) //	Class constructor with two integers indicating the size of the maze and two pairs of integers indicating the coordinates of the starting / ending points
		: mMazeWidth(width), mMazeHeight(height), mStartPos({ start.first , start.second }), mEndPos({ end.first , end.second })
	{
		Create();
	}

	// ===	Creating Stuff	===
protected:
	void Create() //Creates the Maze
	{
		//	===	Define Algorithm (Backtracking) variables	===
		int  mNumVisitedCells;	//	Number of visited cells
		std::stack<std::pair<int, int>> mPath;	// (x, y) coordinate pairs of the path

		//	===	initiates the maze	===
		mMaze = new char* [mMazeHeight];
		mMaze[0] = new char[mMazeWidth * mMazeHeight];

		std::memset(mMaze[0], 0x00, mMazeWidth * mMazeHeight * sizeof(char));
		for (int i = 1; i != mMazeHeight; i++)
			mMaze[i] = mMaze[i - 1] + mMazeWidth;

		// Choose a starting cell for creating the maze (randomlly)
		int x = rand() % mMazeWidth;
		int y = rand() % mMazeHeight;

		mPath.push(std::make_pair(x, y));
		mMaze[y][x] = CELL_VISITED;
		mNumVisitedCells = 1;

		//	===	Do Maze Algorithm	===
		while (mNumVisitedCells < mMazeWidth * mMazeHeight)
		{
			// Create a set of unvisted neighbours
			std::vector<char> neighbours; // change to char

			// North neighbour
			if (mPath.top().second > 0 && !(mMaze[y - 1][x] & CELL_VISITED))
				neighbours.push_back(0);
			// East neighbour
			if (mPath.top().first < mMazeWidth - 1 && !(mMaze[y][x + 1] & CELL_VISITED))
				neighbours.push_back(1);
			// South neighbour
			if (mPath.top().second < mMazeHeight - 1 && !(mMaze[y + 1][x] & CELL_VISITED))
				neighbours.push_back(2);
			// West neighbour
			if (mPath.top().first > 0 && !(mMaze[y][x - 1] & CELL_VISITED))
				neighbours.push_back(3);

			// Are there any neighbours available?
			if (!neighbours.empty())
			{
				// Choose one available neighbour at random
				char next_cell_dir = neighbours[rand() % neighbours.size()];

				// Create a path between the neighbour and the current cell
				switch (next_cell_dir)
				{
				case 0: // North
					mMaze[y - 1][x] |= CELL_VISITED | CELL_PATH_S;
					mMaze[y][x] |= CELL_PATH_N;
					mPath.push(std::make_pair((mPath.top().first + 0), (mPath.top().second - 1)));
					break;

				case 1: // East
					mMaze[y][x + 1] |= CELL_VISITED | CELL_PATH_W;
					mMaze[y][x] |= CELL_PATH_E;
					mPath.push(std::make_pair((mPath.top().first + 1), (mPath.top().second + 0)));
					break;

				case 2: // South
					mMaze[y + 1][x] |= CELL_VISITED | CELL_PATH_N;
					mMaze[y][x] |= CELL_PATH_S;
					mPath.push(std::make_pair((mPath.top().first + 0), (mPath.top().second + 1)));
					break;

				case 3: // West
					mMaze[y][x - 1] |= CELL_VISITED | CELL_PATH_E;
					mMaze[y][x] |= CELL_PATH_W;
					mPath.push(std::make_pair((mPath.top().first - 1), (mPath.top().second + 0)));
					break;

				}

				mNumVisitedCells++;
			}
			else
			{
				// No available neighbours so backtrack!
				mPath.pop();
			}
			x = mPath.top().first;
			y = mPath.top().second;
		}
		//	Highlight the starting/ending points
		mMaze[mStartPos.second][mStartPos.first] |= CELL_START;
		mMaze[mEndPos.second][mEndPos.first] |= CELL_END;

	}


	// ===	Solving STUFF	===
protected:
	void HighLightPath(std::stack<std::pair<int, int>> mPath) {
		while (!mPath.empty())
		{
			mMaze[mPath.top().second][mPath.top().first] |= CELL_SOLUTION;
			mPath.pop();
		}
	}
public:
	void Solve() {// Solve Maze

		//	===	Clear the VISITED Flag	===
		for (int i = 0; i < mMazeHeight; i++)
			for (int j = 0; j < mMazeWidth; j++)
				mMaze[i][j] &= 0xEF;

		//	===	Initiate the Solver	===
		int x = mStartPos.first, y = mStartPos.second;
		std::stack<std::pair<int, int>> mPath;	// (x, y) coordinate pairs of the path
		mPath.push(std::make_pair(x, y));
		mMaze[y][x] |= CELL_VISITED;

		//	===	Backtracking	===
		while (mPath.top() != mEndPos)
		{
			// Create a set of unvisted neighbours
			std::vector<char> neighbours; // change to char

			// North neighbour
			if ((mMaze[y][x] & CELL_PATH_N) && !(mMaze[y - 1][x] & CELL_VISITED))
				neighbours.push_back(0);
			// East neighbour
			if ((mMaze[y][x] & CELL_PATH_E) && !(mMaze[y][x + 1] & CELL_VISITED))
				neighbours.push_back(1);
			// South neighbour
			if ((mMaze[y][x] & CELL_PATH_S) && !(mMaze[y + 1][x] & CELL_VISITED))
				neighbours.push_back(2);
			// West neighbour
			if ((mMaze[y][x] & CELL_PATH_W) && !(mMaze[y][x - 1] & CELL_VISITED))
				neighbours.push_back(3);
			// Are there any neighbours available?
			if (!neighbours.empty())
			{
				// Choose one available neighbour at random
				char next_cell_dir = neighbours[rand() % neighbours.size()];

				// Create a path between the neighbour and the current cell
				switch (next_cell_dir)
				{
				case 0: // North
					mPath.push(std::make_pair((mPath.top().first + 0), (mPath.top().second - 1)));
					break;

				case 1: // East
					mPath.push(std::make_pair((mPath.top().first + 1), (mPath.top().second + 0)));
					break;

				case 2: // South
					mPath.push(std::make_pair((mPath.top().first + 0), (mPath.top().second + 1)));
					break;

				case 3: // West
					mPath.push(std::make_pair((mPath.top().first - 1), (mPath.top().second + 0)));
					break;
				}
			}
			else
			{
				// No available neighbours so backtrack!
				mPath.pop();
			}
			x = mPath.top().first;
			y = mPath.top().second;
			mMaze[y][x] |= CELL_VISITED;
		}

		// specifies the chosen path coordinates
		HighLightPath(mPath);
	}	// Solve Maze

		// ===	DRAWING STUFF	===
protected:

public:
	void Draw() {// Draw Maze

		//	===	Define Drawing variables	===
		const short layoutHeight = mMazeHeight * 2 + 1;
		const short layoutWidth = mMazeWidth * 4 + 1;
		//effective way to define a dynamic array
		char** layoutMaze = new char* [layoutHeight];
		layoutMaze[0] = new char[layoutWidth * layoutHeight];
		std::memset(layoutMaze[0], ' ', layoutWidth * layoutHeight * sizeof(char));	//	initiat the layout array with ' '
		for (int i = 1; i != layoutHeight; i++)
			layoutMaze[i] = layoutMaze[i - 1] + layoutWidth;

		//	===	Fill the Pixels	===
		for (int i = 0; i < mMazeHeight; i++)
		{
			for (int j = 0; j < mMazeWidth; j++)
			{
				//	get the cell situation
				char cellValue = mMaze[i][j];

				//	get the new coordinates
				int y = i * 2;
				int x = j * 4;

				//	fill the top left cell with '+'
				layoutMaze[y][x] = '+';

				//	fill the western wall '|'
				if (!(cellValue & CELL_PATH_W))
					layoutMaze[y + 1][x] = '|';

				//	fill the northern wall "---"
				if (!(cellValue & CELL_PATH_N)) {
					layoutMaze[y][x + 1] = '-';
					layoutMaze[y][x + 2] = '-';
					layoutMaze[y][x + 3] = '-';
				}

				//	fill the solution path with char(254)
				if ((cellValue & CELL_SOLUTION))
					layoutMaze[y + 1][x + 2] = 254;

				//	fill the source cell  with 'A'
				if ((cellValue & CELL_START))
					layoutMaze[y + 1][x + 2] = 'A';

				//	fill the goal cell  with 'Z'
				if ((cellValue & CELL_END)) {	// The goal Cell
					layoutMaze[y + 1][x + 2] = 'Z';
				}
			}
		}

		//	add the eastern border of the maze
		for (int i = 0; i < layoutHeight; i++)
			if (i % 2)
				layoutMaze[i][layoutWidth - 1] = '|';
			else
				layoutMaze[i][layoutWidth - 1] = '+';

		//	add the southern border of the maze
		for (int i = 0; i < layoutWidth; i++)
			if (i % 4)
				layoutMaze[layoutHeight - 1][i] = '-';
			else
				layoutMaze[layoutHeight - 1][i] = '+';

		//		Print on console	
		for (int i = 0; i < layoutHeight; i++)
		{
			for (int j = 0; j < layoutWidth; j++)
				std::cout << layoutMaze[i][j];
			std::cout << std::endl;
		}

		//	Seprate between mazes
		std::cout << "\n====================================================================================================================================\n\n";

		// clear the memory reserved for the layout array
		delete[] layoutMaze[0];
		delete[] layoutMaze;
	}
};

const int MAZE_MAX_WIDTH = 59;	//	the limit has been defined according to console width
const int MAZE_MAX_HEIGHT = 100;
int main()
{
	// Seed random number generator
	srand(time(0));

	/*	Create the Maze	*/
	//SaeedMaze maze(15, 15,{0,0},{14,14});	//	specify the maze size (MAZE_MAX_WIDTH = 59, MAZE_MAX_HEIGHT = 100) and specify the starting/ending cells (<MAZE_MAX_WIDTH-1, <MAZE_MAX_HEIGHT-1)
	SaeedMaze maze(10, 10);	//	specify the maze size (MAZE_MAX_WIDTH = 59, MAZE_MAX_HEIGHT = 100)
	//SaeedMaze maze;	//	Randomlly

	/*	Draw the Maze	*/
	maze.Draw();

	/*	Solve the Maze	*/
	maze.Solve();

	/*	Draw the Maze	*/
	maze.Draw();
	return 0;
}