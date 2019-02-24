/*
 * adjacency.cpp
 *
 *  Created on: 20 Mar 2018
 *      Author: samgl
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>

using namespace std;

int getHighestNumber(string);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "Argument must be an input file." << endl;
		return 1;
	}

	string filename = argv[1];

	ifstream input(filename);

	if (!input)
	{
		cerr << "File " << filename << " not found!" << endl;
		input.close();
		return 1;
	}

	input.close();

	int highestNode = getHighestNumber(filename);

	bool adjacencyArray[highestNode+1][highestNode+1];

	for (int i=0; i<=highestNode; i++)
	{
		for (int j=0; j<=highestNode; j++)
		{
			adjacencyArray[i][j] = false;
		}
	}

	ifstream finalInput(filename);
	string line;

	while (getline(finalInput, line))
	{
		int index;

		for (unsigned i=0; i<line.length(); i++)
		{
			if (line.at(i) == ' ')
			{
				index = i;
			}
		}

		string first = "";
		string second = "";

		for (int i=0; i<index; i++)
		{
			first += line[i];
		}

		for (unsigned i=index+1; i<line.length(); i++)
		{
			second += line[i];
		}

		int firstNumber = atoi(first.c_str());
		int secondNumber = atoi(second.c_str());

		adjacencyArray[firstNumber][secondNumber] = true;
	}

	finalInput.close();

	int count = 0;

	int sent[highestNode+1][2];
	int received[highestNode+1][2];
	int total[highestNode+1][2];

	for (int i=0; i<=highestNode; i++)
	{
		sent[i][0] = i;
		sent[i][1] = 0;
		received[i][0] = i;
		received[i][1] = 0;
		total[i][0] = i;
	}

	int selfEmails = 0;

	ofstream output("outputMatrix.txt");

	for (int i=0; i<=highestNode; i++)
	{
		for (int j=0; j<=highestNode; j++)
		{
			if (adjacencyArray[i][j])
			{
				if (i == j)
				{
					selfEmails++;
				}
				else
				{
					output << "X";
				}

				sent[i][1]++;
				received[j][1]++;
				count ++;
			}
			else
			{
				output << " ";
			}
		}

		output << endl;
	}

	output.close();

	int noneSent = 0;
	int noneReceived = 0;

	for (int i=0; i<=highestNode; i++)
	{
		total[i][1] = sent[i][1] + received[i][1];

		if (sent[i][1] == 0)
		{
			noneSent++;
		}

		if (received[i][1] == 0)
		{
			noneReceived++;
		}
	}

	bool sorted = false;

	while (!sorted)
	{
		sorted = true;

		for (int i=0; i<highestNode; i++)
		{
			if (sent[i][1] < sent[i+1][1])
			{
				int reference = sent[i][0];
				int num = sent[i][1];
				sent[i][0] = sent[i+1][0];
				sent[i][1] = sent[i+1][1];
				sent[i+1][0] = reference;
				sent[i+1][1] = num;
				sorted = false;
			}
		}
	}

	sorted = false;

	while (!sorted)
	{
		sorted = true;

		for (int i=0; i<highestNode; i++)
		{
			if (received[i][1] < received[i+1][1])
			{
				int reference = received[i][0];
				int num = received[i][1];
				received[i][0] = received[i+1][0];
				received[i][1] = received[i+1][1];
				received[i+1][0] = reference;
				received[i+1][1] = num;
				sorted = false;
			}
		}
	}

	sorted = false;

	while (!sorted)
	{
		sorted = true;

		for (int i=0; i<highestNode; i++)
		{
			if (total[i][1] < total[i+1][1])
			{
				int reference = total[i][0];
				int num = total[i][1];
				total[i][0] = total[i+1][0];
				total[i][1] = total[i+1][1];
				total[i+1][0] = reference;
				total[i+1][1] = num;
				sorted = false;
			}
		}
	}

	ofstream sentOutput("sentOutput.txt");
	sentOutput << "Node#\tSent" << endl;

	for (int i=0; i<=highestNode; i++)
	{
		if (sent[i][0] < 1000)
		{
			sentOutput << sent[i][0] << "\t\t" << sent[i][1] << endl;
		}
		else
		{
			sentOutput << sent[i][0] << "\t" << sent[i][1] << endl;
		}

	}

	sentOutput.close();

	ofstream receivedOutput("receivedOutput.txt");
	receivedOutput << "Node#\tReceived" << endl;

	for (int i=0; i<=highestNode; i++)
	{
		if (received[i][0] < 1000)
		{
			receivedOutput << received[i][0] << "\t\t" << received[i][1] << endl;
		}
		else
		{
			receivedOutput << received[i][0] << "\t" << received[i][1] << endl;
		}

	}

	receivedOutput.close();

	ofstream totalOutput("totalOutput.txt");
	totalOutput << "Node#\tTotal" << endl;

	for (int i=0; i<=highestNode; i++)
	{
		if (total[i][0] < 1000)
		{
			totalOutput << total[i][0] << "\t\t" << total[i][1] << endl;
		}
		else
		{
			totalOutput << total[i][0] << "\t" << total[i][1] << endl;
		}

	}

	totalOutput.close();

	cout << "Total emails sent between " << highestNode+1 << " people: " << count << endl;

	if (selfEmails > 0)
	{
		int uniqueEmails = count - selfEmails;
		cout << "This included " << selfEmails << " emails sent from any node to itself." << endl;
		cout << "There were " << uniqueEmails << " emails sent between unique nodes." << endl;
	}

	cout << "Most emails sent: Node " << sent[0][0] << " with " << sent[0][1] << " emails." << endl;
	cout << "Most emails received: Node " << received[0][0] << " with " << received[0][1] << " emails." << endl;
	cout << "Node with most emails sent/received: Node " << total[0][0] << " with " << total[0][1] << " emails." << endl;
	cout << noneSent << " nodes sent no emails." << endl;
	cout << noneReceived << " nodes received no emails." << endl;

	float sentPercent = ((float)(sent[0][1])/(float)(count)) * 100;
	float formatSent = roundf(sentPercent * 100) / 100;
	float receivedPercent = ((float)(received[0][1])/(float)(count)) * 100;
	float formatReceive = roundf(receivedPercent * 100) / 100;
	float totalPercent = ((float)(total[0][1])/(float)(count)) * 100;
	float formatTotal = roundf(totalPercent * 100) / 100;

	cout << "Node " << sent[0][0] << " sent " << formatSent << "% of all emails." << endl;
	cout << "Node " << received[0][0] << " received " << formatReceive << "% of all emails." << endl;
	cout << "Node " << total[0][0] << " was responsible for " << formatTotal << "% of all emails." << endl;

	return 0;
}

int getHighestNumber(string filename)
{
	int highestNode = 0;
	string line;
	stringstream stream;
	ifstream newInput(filename);
	string numbers[2] = {"", ""};

	while (getline(newInput, line))
	{
		int index;

		for (unsigned i=0; i<line.length(); i++)
		{
			if (line.at(i) == ' ')
			{
				index = i;
			}
		}

		string first = "";
		string second = "";

		for (int i=0; i<index; i++)
		{
			first += line[i];
		}

		for (unsigned i=index+1; i<line.length(); i++)
		{
			second += line[i];
		}

		int firstNumber = atoi(first.c_str());
		int secondNumber = atoi(second.c_str());

		if (firstNumber > highestNode)
		{
			highestNode = firstNumber;
		}

		if (secondNumber > highestNode)
		{
			highestNode = secondNumber;
		}
	}

	newInput.close();

	return highestNode;
}
