#include "traitement.hh"

//Return vitzone * 1000 + maxvit
//vitzone => vitesse dans la zone
//maxvit => vitesse maximale autorisée

int traitementImage(std::vector<Mat> panneaux)
{
	int vitzone = 90;
	int maxvit;

	//if (img.data)
	//{
	/*	if (vitzone == 90 && isCity(img))
			vitzone = maxvit = 50;
		else if (vitzone == 90 && isHighWay(img))
			vitzone = maxvit =  130;
		else if (vitzone == 50 && endCity(img))
			vitzone = maxvit = 90;
		cout << "Vitesse maximale dans cette zone: " << vitzone << endl;*/
		/******* Detection d'un panneau de limitation *******/
		/*std::vector<Circle*> circles = getCirclesByEllipses(img);
		std::vector<Mat> panneaux;
		for (std::vector<Circle*>::iterator it = circles.begin();
				it != circles.end(); it++){
			Mat* m = isLimitation(img, *it);
			if (m != NULL)
				panneaux.push_back(*m);
		}*/
		std::vector<int> matches = std::vector<int>();
		for (std::vector<Mat>::iterator it = panneaux.begin();
				it != panneaux.end(); it++){
			DIR* d;
			cvtColor(*it, *it, CV_BGR2GRAY);
			d = opendir("./modeles/vitesse");
			if (!d)
				exit(1);
			struct dirent* f;
			while ((f = readdir(d)))
			{
				string st = "./modeles/vitesse/";
				if (f->d_name[0] != '.')
				{
					st += f->d_name;
					cout << st << endl;
					Mat mod = imread(st);
					if ((*it).data)
					{
						Mat outImg = *it;
						if (outImg.data)
						{
							cvtColor(mod, mod, CV_BGR2GRAY);
							int n = findObject(*it, mod, 500, Scalar(255,0,0), outImg);
							matches.push_back(n);
							cout <<"nombre match: "<< n << endl;
						}
					}
				}
			}
			int minpos = 0;
			for (int i = 1; i < matches.size(); i++)
			{
				if (matches[i] > matches[minpos])
					minpos = i;
			}
			if (matches[minpos] == 0)
				continue;
			switch(minpos)
			{
				case 0: maxvit = 130; break;
				case 1: maxvit = 90; break;
				case 2: maxvit = 30; break;
				case 3: maxvit = 70; break;
				case 4: maxvit = 110; break;
				default: maxvit = 50;
			}
			cout << "Vitesse maximale autorisée: " << maxvit << endl;
			return vitzone*1000 + maxvit + 1000000 * matches[minpos];
		}
		return 0;
//	}
/*	else
	{
		std::cerr << "Ouverture de l'image impossible" << std::endl;
		exit(1);
	}*/
}

