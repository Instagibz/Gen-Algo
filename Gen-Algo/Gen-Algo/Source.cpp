#include <Windows.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>



//lets create a population
struct Member {
	std::string DNA;
	int Fitness;
};

struct Population {
	std::vector<Member> Members = std::vector<Member>(5000); // this will create a population with 50k members, lets choose a lower number first to see the progress, works fine...
};


int main() {
	//for this example we are going to use a string as a kind of "goal" to achieve
	std::string DNA = "I am a DNA string that needs to be found, shall happen soon!";
	bool SequenceFound = false;
	int MutationRate = 25; // We are going to use this for mutation of genes 1000 = 100% 1 = .01% mutation rate
	srand(time(NULL));

	//create a population and initialize it with random DNA, also set the fitness to 0
	Population Pop;

	for (int i = 0; i < Pop.Members.size(); i++) {
		Pop.Members.at(i).DNA.resize(DNA.size());
		for (int j = 0; j < DNA.size(); j++) { Pop.Members.at(i).DNA.at(j) = (unsigned char)rand() % 96 + 32; }
		Pop.Members.at(i).Fitness = 0;
	}

	//we will use a variable to keep track of how many generations there have been
	int Generation = 0;

	//our actuall work will be inside this loop, it will stop when the DNA sequence has fully evolved to the target
	while (!SequenceFound) {
		Generation++;

		//clear out the fitness here and then reevaluate for each member, also check if fitness has reached maximum
		for (int i = 0; i < Pop.Members.size(); i++) {
			Pop.Members.at(i).Fitness = 0;
			for (int j = 0; j < Pop.Members.at(i).DNA.size(); j++) { if (Pop.Members.at(i).DNA.at(j) == DNA.at(j)) { Pop.Members.at(i).Fitness += 10; } }
			if (Pop.Members.at(i).Fitness == DNA.size() * 10) SequenceFound = true;
		}

		//now lets sort the population by fitness, from highest to lowest
		std::sort(Pop.Members.begin(), Pop.Members.end(), [](Member const &a, Member &b) {return a.Fitness > b.Fitness; });

		//select x amount of highest fitness members to pair from, lets use 2 parents in this case
		std::vector<Member> Parents{ Pop.Members.at(0), Pop.Members.at(1) };

		//lets do some gene permutation and mating 
		for (int i = 0; i < Pop.Members.size(); i++) {

			for (int j = 0; j < Pop.Members.at(i).DNA.size(); j++) {

				//lets use an equal chance to take each parents gene (on a per gene basis)
				int TempSelection = rand() % Parents.size();
				Pop.Members.at(i).DNA.at(j) = Parents.at(TempSelection).DNA.at(j);

				//dont forget to apply random mutation based on our value from above
				if (rand() % 1000 < MutationRate) { Pop.Members.at(i).DNA.at(j) = (unsigned char)rand() % 96 + 32; }
			}
		}

		//lets print some stuff
		std::cout << "Generation : " << Generation << " Highest Fitness : " << Parents.at(0).Fitness << " With Sequence : " << Parents.at(0).DNA.c_str() << std::endl;
	}

	std::cout << "Generation " << Generation << " Evolved to the full sequence" << std::endl;
	Sleep(2000);
	return 0;
}

//done... :)