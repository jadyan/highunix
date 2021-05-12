class Timer
{
public:
	explicit Timer(int tickFrequency);

	// Automatically called for each tick.
	virtual void onTick() const;
};

class Widget
{
private:
	class WidgetTimer : public Timer
	{
	public:
		virtual void onTick() const;
	};

	WidgetTimer timer;
};

int main()
{
	return 0;
}