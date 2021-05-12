class Timer
{
public:
	explicit Timer(int tickFrequency);

	// Automatically called for each tick.
	virtual void onTick() const;
};

class Widget : private Timer
{
private:
	// Look at Widget data etc.
	virtual void onTick() const;
};

int main()
{
	return 0;
}