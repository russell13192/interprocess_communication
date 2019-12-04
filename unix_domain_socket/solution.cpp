int read4(char* buf);

class Solution
{

public:

	queue<char> q; // Collection of unused chars last time
	int read(char* buf, int n)
	{
		int total = 0; // Current number of characters you get
		char* ptr = buf; // Copy construct the buffer ptr
		int count = 1;
		while (!q.empty() && total < n)
		{
			q.pop();
			++ptr;
			total += 1;
		}
		if (total == n)
		{
			return total;
		}
		while (total < n && count != 0)
		{
			count = read4(buf + total);
			total += count;
		}
		if (total <= n)
		{
			return total;
		}

		int num_delete = total - n;
		ptr = buf + total - num_delete;

		for (int i = 0; i < num_delete; ++i)
		{
			/* code */
			q.push(*ptr);
			++ptr;
		}
		return total;

	}
};