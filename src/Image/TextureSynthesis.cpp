#include "TextureSynthesis.h"

ImageData* TextureSynthesis::synth(const ImageData * sample, int wdt /*= 640*/, int hgt /*= 480*/, int nr)
{
	int srcW = sample->getWidth();
	int srcH = sample->getHeight();

	ImageData* ret = new ImageData(srcW, srcH);
	int neighSize = 2 * (sqr(nr) + nr);

	// Create vector map for sample image
	Vector3f** smpVec;
	Aligned_2DArray(smpVec, srcW, srcH);
	for (int v = 0; v < srcH; v++)
	{
		for (int u = 0; u < srcW; u++)
		{
			smpVec[v][u] = ColorToVector(sample->getRGBA(u, v));
				//Vector3D(sample.pixels[u][v].r, sample.pixels[u][v].g, sample.pixels[u][v].b);
			//smpVec[u + v * srcW].getInfo();
		}
	}
	// End of vector map
	// Timer
	clock_t startT, endT;
	startT = clock();
	for (int j = nr; j < srcH - nr; j++)
	{
		clock_t lineS, lineE;
		lineS = clock();
		for (int i = nr; i < srcH - nr; i++)
		{
			// Store neighbor pixels
			Vector3f* neighPixs = new Vector3f[neighSize];
			for (int n = -nr; n <= 0; n++)
			{
				for (int m = -nr; m <= nr; m++)
				{
					if (n == 0 && m == 0)
					{
						break;
					}
					else
					{
						//cout << (nr + nr + 1) * n + m + neighSize << endl;
						
						neighPixs[((nr << 1) + 1) * n + m + neighSize] = ColorToVector(sample->getRGBA(i + m, j + n));
							//Vector3D(src->pixels[i + m][j + n].r, src->pixels[i + m][j + n].g, src->pixels[i + m][j + n].b);
					}
				}
			}
			// Find most similar pixel in sample image
			Float vecSum, minSum;
			Vector3f tmpVec, minVec;
			minSum = 442;//442 is sqrt(225*255*3)
			for (int v = 0; v < srcH; v++)
			{
				for (int u = 0; u < srcW; u++)
				{
					//cout << u << ", " << v << endl;
					vecSum = 0;

					for (int n = -nr; n <= 0; n++)
					{
						for (int m = -nr; m <= nr; m++)
						{
							if (n == 0 && m == 0)
							{
								break;
							}
							else
							{
								int uIndex = u + m < 0 ? 0 : (u + m >= srcW ? srcW : u + m);
								int vIndex = v + n < 0 ? 0 : (v + n >= srcH ? srcH : v + n);
								tmpVec = neighPixs[(nr * 2 + 1)*n + m + neighSize] - smpVec[vIndex][uIndex];
								//cout << tmpVec.getLength() << endl;
								vecSum += tmpVec.length() / neighSize;
								//vecSum += (neighPixs[(nr * 2 + 1)*n + m + neighSize] ).getLength() / neighSize;
								//cout << "\t" << (nr * 2 + 1)*n + m + neighSize << endl;
								//cout << "\t" << m << "\t" << n << endl;
							}
						}
					}
					//cout << vecSum << endl;
					if (vecSum < minSum)
					{
						minSum = vecSum;
						minVec = smpVec[v][u];
					}
				}
			}
			//cout << "\t" << minSum << endl;
			ret->setRGBA(i, j, ColorRGBA(minVec.x, minVec.y, minVec.z));
			//minSum = 442;
			delete[] neighPixs;

		}
		lineE = clock();
		std::cout << "Line " << j << " done in " << (lineE - lineS) / CLOCKS_PER_SEC << " sec." << endl;
		//ret->pixels[nr][j].printData();
	}
	endT = clock();
	std::cout << "Timer synthesis runtime :" << (endT - startT) / CLOCKS_PER_SEC << " sec." << endl;

	return ret;
}
