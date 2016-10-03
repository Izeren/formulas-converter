
class CRect {
public:
	CRect();
	CRect(int _left, int _top, int _right, int _bottom);

	void Set(int left, int top, int right, int bottom);
	bool IsContain(int x, int y) const;
	bool isIntersect(const CRect& other) const;

	CRect& CRect::operator= (const CRect& rect);

	bool operator==(const CRect& rect) const;

	int& Left();
	int& Right();
	int& Top();
	int& Bottom();
	const int Left() const;
	const int Right() const;
	const int Top() const;
	const int Bottom() const;

	const int GetWidth() const;
	const int GetHeight() const;

private:
	int left;
	int top;
	int right;
	int bottom;
};

