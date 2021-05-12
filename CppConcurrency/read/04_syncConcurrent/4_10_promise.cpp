/*
 * @Author: your name
 * @Date: 2021-02-03 16:32:20
 * @LastEditTime: 2021-02-03 16:59:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/4.10.cpp
 */
#include <iostream>
#include <future>
#include <vector>

using namespace std;


typedef int payload_type;
typedef int outgoing_data;

struct data_packet
{
	int id;
	payload_type payload;
	std::promise<bool> promise;
};

struct connection
{
public:
	data_packet incoming(){}
	bool has_incoming_data(){}
	bool has_outgoing_data(){}
	std::promise<payload_type>& get_promise(int d){}
	data_packet top_of_outgoing_queue(){}
	void send(payload_type p){}
};

std::vector<connection> connection_set;
bool done(std::vector<connection>&)
{
    return false;
};



/*
函数process_connections()中，直到done()返回true①为止。
每一次循环，都会依次的检查每一个连接
②，检索是否有数据
③或正在发送已入队的传出数据
⑤。假设输入数据包是具有ID和有效负载的(有实际的数在其中)。
一个ID映射到一个std::promise(可能是在相关容器中进行的依次查找)
④，并且值是设置在包的有效负载中的。
对于传出包，包是从传出队列中进行检索的，实际上从接口直接发送出去。当发送完成，
与传出数据相关的承诺值将置为true，来表明传输成功⑥。是否能映射到实际网络协议上，
取决于网络所用协议；这里的“承诺值/期望值”组合方式可能在特殊的情况下无法工作，但它与一些操作系统支持的异步输入/输出结构类似。

*/
void process_connections(std::vector<connection>& connections)
{
	while (!done(connections)) // (1)
	{
		for (auto connection_b = begin(connections), connection_e = end(connections); connection_b != connection_e; ++connection_b)
		{
			if (connection_b->has_incoming_data()) // (3)
			{
				data_packet data = connection_b->incoming();
				std::promise<payload_type>& p =
					connection_b->get_promise(data.id); // (4)
				p.set_value(data.payload);
			}
			if (connection_b->has_outgoing_data()) // (5)
			{
				data_packet out_data =
					connection_b->top_of_outgoing_queue();
				connection_b->send(out_data.payload);
				out_data.promise.set_value(true); // (6)
			}
		}
		
	}
}




int main()
{
    connection conn1;
    connection conn2;
    connection conn3;
    std::vector<connection>  connections;
    connections.push_back(conn1);
    connections.push_back(conn2);
    connections.push_back(conn3);

    process_connections(connections);
}
