#include "traitement.hh"

//Return vitzone * 1000 + maxvit
//vitzone => vitesse dans la zone
//maxvit => vitesse maximale autorisée

int traitementImage(vector<Mat> panneaux, vector<vector<sForme> > forms)
{
    int vitzone = 90;
    int maxvit;
    
    cout << forms.size() << endl;
    if (forms.size() == 0)
        return 0;

    std::vector<int> matches = std::vector<int>();
    for (int it = 0; it < panneaux.size(); it++){
        DIR* d;
        vector<sForme> forme = forms[it];
        int nb = forme.size();
        if (nb <= 1)
            return 0;
        Mat im = panneaux[it];
        if (nb == 2)
            im = Mat(panneaux[it], Rect(forme[0].minx, forme[0].miny, forme[0].largeur, forme[0].hauteur));
        else
            im = Mat(panneaux[it], Rect(forme[0].minx, forme[0].miny, forme[1].maxx - forme[0].minx, forme[1].hauteur));
        Adaptbinar(im);
        //BlackFilter(*it);
        d = opendir("./modeles/vitesse");
        if (!d)
            exit(1);
        struct dirent* f;
        while ((f = readdir(d)))
        {
            string st = "./modeles/vitesse/";
            if (f->d_name[0] != '.')
            {
                if ((nb == 2 && f->d_name[0] != '1') || (nb == 3 && f->d_name[0] == '1'))
                {
                    st += f->d_name;
                    cout << st << endl;
                    Mat mod = imread(st);
                    if (im.data)
                    {
                        Mat outImg = im;
                        if (outImg.data)
                        {
                            //BlackFilter(*it);
                            Adaptbinar(mod);
                            int n = findObject(im, mod, 500, Scalar(255,0,0), outImg);
                            matches.push_back(n);
                            cout <<"nombre match: "<< n << endl;
                        }
                    }
                }
                else
                    matches.push_back(0);
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

