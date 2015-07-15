#include "ransac.hh"

vector<Point> get3Points(vector<Point> cnt, int seed)
{
	srand(seed);
	vector<Point> point3 = vector<Point>();
	while (point3.size() < 3)
	{
		int x = rand() % cnt.size();
		point3.push_back(cnt[x]);
	}
	return point3;	
}

vector<int> geteq(Point center, vector<Point> point3)
{
	int xc = center.x;
	int yc = center.y;
	int x1 = point3[0].x - xc;
	int y1 = point3[0].y - yc;
	int x2 = point3[1].x - xc;
	int y2 = point3[1].y - yc;
	int x3 = point3[2].x - xc;
	int y3 = point3[2].y - yc;
	int divc = (y3^2) - ((y2^2)*(x1^2) - (y1^2))*x1*y1/(x2*y2*(x1^2) - (x2^2)*x1*y1) - (((y2^2) - (y1^2)/(x1^2))*x1*y1/(x2*y2*(x1^2) - (x2^2)*x1*y1) - (y1^2)/(x1^2))*(x3^2);
	int numc = 1 - ((x1^2) - 1)*x3*y3/(x2*y2*(x1^2) - (x2^2)*x1*y1) - (1/(x1^2) - (1 - 1/(x1^2))*x1*y1/(x2*y2*(x1^2) - (x2^2)*x1*y1)) * (x3^2);
	int c = numc / divc;
	int b = ((x1^2) - 1)/(x2*y2*(x1^2) - (x2^2)*x1*y1) - c * ((y2^2) * (x1^2) - (y1^2))/(x2*y2*(x1^2) - (x2^2)*x1*y1);
	int a = (1 - b * x1*y1 - c * (y1^2))/(x1^2);
	vector<int> eq = vector<int>();
	eq.push_back(a);
	eq.push_back(b);
	eq.push_back(c);
	return eq;
}
