#include "Ghost.h"
#include "Game.h"

void Ghost::move(array <array<bool, MAXCOLS>, MAXROWS>& food, array <array<bool, MAXCOLS>, MAXROWS>& walls,
	char& level, const Point& pacman, vector <Ghost>& brotherGhosts)
{
	switch (level)
	{
	case 'a': // BEST
		BestMove(walls, food, body, pacman, brotherGhosts);
		break;
	case 'b': // GOOD
			if	  (Game::getMoveCounter() % 20 == 0
				|| Game::getMoveCounter() % 21 == 0
				|| Game::getMoveCounter() % 22 == 0
				|| Game::getMoveCounter() % 23 == 0
				|| Game::getMoveCounter() % 24 == 0)
				{
					simpleMove(food, walls); // need to take a break.. execution of case 'c';
				} 
			else 
			{ 
				BestMove(walls, food, body, pacman, brotherGhosts);
			}
				
		break;
	case 'c': // DUMB_ASS

		simpleMove(food, walls);
		break;

	default:
		break;
	}
}

void Ghost::printGhost()const
{
	if (Game::getSilentMode())
		return;

	if (Board::IsColorOn())
		setTextColor(Color::RED);

	body.draw(GHOST); // print new point
	hideCursor();
	//setTextColor(Color::WHITE);
}

void Ghost::BestMove(array <array<bool, MAXCOLS>, MAXROWS>& walls, array <array<bool, MAXCOLS>, MAXROWS>& food,
	Point src, Point dest, vector <Ghost>& brotherGhosts)
{
	Point saver = this->body;

	if (isFoodInXY(food, body)) {
		if (Board::IsColorOn())
			setTextColor(Color::WHITE);
		body.draw(FOOD); // create effect of a ghost flying over the food 
		hideCursor();
	}
	else
	{
		body.draw(' '); // delete last point
		hideCursor();
	}
	if (Game::getMode() == GameMode::LOAD)
	{
		direction = (int)Steps.front();
		Steps.pop_front();
		body.moveWIthDirection(direction);
	}
	else // SaveMode
	{
		// Stores the smallest path
		string path = "";
		pathMoves(walls, src, dest, path);

		switch (path[0])
		{
		case 'U':
			direction = (int)Direction::UP;
			break;
		case 'D':
			direction = (int)Direction::DOWN;
			break;
		case 'L':
			direction = (int)Direction::LEFT;
			break;
		case 'R':
			direction = (int)Direction::RIGHT;
			break;
		default:
			break;
		};

		body.moveWIthDirection(direction);
		// callision check;
		for (unsigned int i = 0; i < brotherGhosts.size(); i++)
		{
			if (this->body == brotherGhosts[i].body && this != &brotherGhosts[i])
			{
				body = saver;
				direction = (int)Direction::STAY;
				body.moveWIthDirection(direction);
			}
		}
		Steps.push_back(direction);
	}
	printGhost();
}

void Ghost::pathMoves(array <array<bool, MAXCOLS>, MAXROWS>& mat,Point src, Point dest, string& path)const
{
	int dRow[] = { -1, 0, 0, 1 };
	int dCol[] = { 0, -1, 1, 0 };

	// Stores the distance for each
	// cell from the source cell
	int d[MAXROWS][MAXCOLS];
	memset(d, -1, sizeof d);

	// Distance of start cell is 0
	d[src.GetY()][src.GetX()] = 0;

	// Initialize a visited array to track the positions we are going to explore
	bool visited[MAXROWS][MAXCOLS];
	memset(visited, false, sizeof visited);

	// Mark source cell as visited
	visited[src.GetY()][src.GetX()] = true;

	// Create a queue for BFS algoritem
	queue<Node> q;
	// Distance of source cell is 0
	Node s = { src, 0 };
	// Enqueue source cell
	q.push(s);
	// Keeps track of whether pacman has been reached or not.
	bool missionAcomplished = false;
	// Iterate until queue is not empty
	while (!q.empty())
	{

		// Deque front of the queue
		Node curr = q.front();
		Point p = curr.pt;

		// If the destination cell is reached, then find the path
		if (p.GetX() == dest.GetX() && p.GetY() == dest.GetY())
		{
			int x = p.GetX(), y = p.GetY();
			int dist = curr.dist;
			// Assign the distance of destination to the distance matrix
			d[p.GetY()][p.GetX()] = dist;
			// Iterate until source is reached
			while (x != src.GetX()	|| y != src.GetY())
			{

				// Append DOWN
				if (y > 0 && d[y - 1][x] == dist - 1) 
				{
					path += 'D';
					y--;
				}

				// Append UP
				if (y < MAXROWS - 1 && d[y + 1][x] == dist - 1)
				{
					path += 'U';
					y++;
				}

				// Append RIGHT
				if (x > 0 && d[y][x - 1] == dist - 1) {
					path += 'R';
					x--;
				}

				// Append LEFT
				if (x < MAXCOLS - 1 && d[y][x + 1]== dist - 1)
				{
					path += 'L';
					x++;
				}
				dist--;
			}

			// Reverse the backtracked path
			reverse(path.begin(), path.end());
			missionAcomplished = true;
			break;
		}
		// Pop the start of queue
		q.pop();
		// Explore all adjacent directions
		for (int i = 0; i < 4; i++)
		{
			int row = p.GetY() + dRow[i];
			int col = p.GetX() + dCol[i];

			// If the current cell is valid
			// cell and can be traversed
			if (isValid(row, col) && (mat[row][col] == false) && !visited[row][col])
			{
				// Mark the adjacent cells as visited
				visited[row][col] = true;

				// Enqueue the adjacent cells
				Node adjCell = { { col, row }, curr.dist + 1 };
				q.push(adjCell);

				// Update the distance
				// of the adjacent cells
				d[row][col] = curr.dist + 1;
			}
		}
	}
	// If the destination is not reachable
	if (!missionAcomplished)
		return;
}

void Ghost::simpleMove(array <array<bool, MAXCOLS>, MAXROWS>& food, array <array<bool, MAXCOLS>, MAXROWS>& walls)
{
	Point saver = body;

	if (Game::getMode() == GameMode::LOAD)
	{
		direction = (int)Steps.front();
		Steps.pop_front();
	}
	else
	{
		if (Game::getMoveCounter() % 20 == 0)
			setRandDirection(); // random direction
	}

	if (isFoodInXY(food, body)) {
		if (Board::IsColorOn())
			setTextColor(Color::WHITE);
		body.draw(FOOD); // create effect of a ghost flying over the food 
		hideCursor();
	}
	else
	{
		body.draw(' '); // delete last point
		hideCursor();
	}
	body.moveWIthDirection(direction);
	if (!IsWallInXY(walls, body))
	{
		printGhost();
	}
	else
	{
		body = saver;
		printGhost();
	}
	if (Game::getMode() == GameMode::SAVE)
		Steps.push_back(direction);
}

void Ghost::ReverseDirection()
{
	switch (direction)
	{
		case (int)Direction::UP:
			setDirection((int)Direction::DOWN);
				break;
		case (int)Direction::DOWN:
			setDirection((int)Direction::UP);
			break;
		case (int)Direction::RIGHT:
			setDirection((int)Direction::LEFT);
			break;
		case (int)Direction::LEFT:
			setDirection((int)Direction::RIGHT);
			break;
	}
}