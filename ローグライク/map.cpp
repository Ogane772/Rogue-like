#include "map.h"
#include "wall.h"
#include "meshfield.h"
#include "CTexture.h"
#include "Cplayer.h"
#include "CEnemy.h"
#include <malloc.h>
#include <memory.h>
#include "CObject.h"
#include "stage.h"

int CMap::GroupWidth;
int CMap::GroupHeight;
//int CMap::g_TexWood2;
CMap::MAP **CMap::g_map, *CMap::base_g_map;

void CMap::Map_Initialize(void)
{
	CEnemy::DeleteAllEnemy();
	//g_TexWood2 = -1;
	if (CStage::Stage_GetLevel() == 1)
	{
		GroupWidth = 2;
		GroupHeight = 2;
	}
	
	if (CStage::Stage_GetLevel() == 2)
	{
		GroupWidth = 3;
		GroupHeight = 2;
	}

	if (CStage::Stage_GetLevel() == 3)
	{
		GroupWidth = 3;
		GroupHeight = 3;
	}

	//g_TexWood2 = Texture_SetLoadFile("./asset/texture/wood.png", 100, 100);

	g_map = (MAP**)malloc(sizeof(MAP *) * MAX_MAPHEIGHT);
	base_g_map = (MAP*)malloc(sizeof(MAP) * MAX_MAPHEIGHT * MAX_MAPWIDTH);

	for (int i = 0; i < MAX_MAPHEIGHT; i++) {
		g_map[i] = base_g_map + i * MAX_MAPWIDTH;
	}

	/*for (int i = 0; i < MAX_MAPHEIGHT; i++) {
		for (int j = 0; j < MAX_MAPWIDTH; j++) {
			g_map[i][j] = i * MAX_MAPWIDTH + j;
			
		}
	}*/

	for (int z = 0; z < MAX_MAPHEIGHT; z++)
	{

		for (int x = 0; x < MAX_MAPWIDTH; x++)
		{
			g_map[z][x].type = 0;		// ���ꖳ��
			g_map[z][x].have = NOTHAVE;
			g_map[z][x].use = false;
			g_map[z][x].Cxwall = false;
			g_map[z][x].Czwall = false;
			g_map[z][x].Cxtopwall = false;
			g_map[z][x].Cxbotwall = false;
			g_map[z][x].CzRwall = false;
			g_map[z][x].CzLwall = false;
			g_map[z][x].pos = D3DXVECTOR3(-247.5f, 0.0f, 247.5f);
			g_map[z][x].pos.x += x * 5.0f;
			g_map[z][x].pos.z += -z * 5.0f;
			//g_map[z][x].Group = x / GroupWidth;
			g_map[z][x].Group = (z / (MAX_MAPHEIGHT / GroupHeight) * GroupWidth) + (x / (MAX_MAPWIDTH / GroupWidth) + 1);	// �O���[�v�t��

		}
	}
}


void CMap::Map_Create(void)
{

	//===================================================
	// �t���A��GroupHeight * GroupWidth����
	//===================================================
	for (int i = 0; i < GroupHeight * GroupWidth; i++)
	{
		
		// �t���A�n�_z��
		int z = rand() % (MAX_MAPHEIGHT / GroupHeight * (i / GroupWidth + 1));
		// �����̂�蒼���`�F�b�N
		if (z <= 4 + MAX_MAPHEIGHT / GroupHeight * (i / GroupWidth) ||
			z >= MAX_MAPHEIGHT / GroupHeight * (i / GroupWidth + 1) - 10)
		{
			i--;
			continue;
		}
		else
		{

			// �t���A�n�_x�� 
			// �~�X
			int x = rand() % (MAX_MAPWIDTH / GroupWidth * (i % GroupWidth + 1));
	//		int x = rand() % (MAX_MAPWIDTH / GroupWidth * ((i + 1) % GroupWidth + 1));
			// �����̂�蒼���`�F�b�N
			if (x <= 4 + (MAX_MAPWIDTH / GroupWidth * (i % GroupWidth)) ||
				x >= MAX_MAPWIDTH / GroupWidth * (i % GroupWidth + 1) - 10)
			{
				i--;
				continue;
			}
			else
			{
				// �t���A�̏c�̒���4�`8
				int height = rand() % 5 + 4 + z;

				// �t���A�̉��̒���4�`8
				int width = rand() % 5 + 4 + x;

				//while (z <= height)
				for (int h = z; h <= height; h++)
				{


					//while (x <= width)
					// �t���A�쐬
					for(int w = x;w <= width;w++)
					{
						g_map[h][w].type = 1;
						g_map[h][w].use = true;
						//x++;
					}
					//z++;
				}

				// �t���A�o�����쐬
				if (i % GroupWidth == 0)
				{
					// �E�Ɖ��ɍ쐬
					if (i == 0)	
					{
						int rightgate = (rand() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}

						int bottomgate = (rand() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
						}
					}
					// �E�Ə�ɍ쐬
					else if (i + GroupWidth == GroupHeight * GroupWidth)
					{
						
						int rightgate = (rand() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}

						int topgate = (rand() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;

						// ��ʘH�̍쐬
						for (int path = z - 1; g_map[path][topgate + x].Group == i + 1; path--)
						{
							// ���ʘH�ƌq����
							if (g_map[path - 1][topgate + x].Group != i + 1)
							{
								// 
								int bottom;
								// ���ʘH��x���W��T��
								for (int bx = 0; bx < MAX_MAPWIDTH; bx++)
								{
									if (g_map[path][bx].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[path][bx].type == 2)
										{
											bottom = bx;
											break;
										}
									}
								}

								// �ʘH������ 
								if (bottom < topgate + x)
								{
									for (int bond = topgate + x;bond != bottom;bond--)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}

							}

							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;

							
						}


					}
					// �E�Ə�Ɖ��ɍ쐬
					else
					{
						int rightgate = (rand() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}

						int bottomgate = (rand() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
						}

						int topgate = (rand() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;

						// ��ʘH�̍쐬
						for (int path = z - 1; g_map[path][topgate + x].Group == i + 1; path--)
						{
							// ���ʘH�ƌq����
							if (g_map[path - 1][topgate + x].Group != i + 1)
							{
								// 
								int bottom;
								// ���ʘH��x���W��T��
								for (int bx = 0; bx < MAX_MAPWIDTH; bx++)
								{
									if (g_map[path][bx].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[path][bx].type == 2)
										{
											bottom = bx;
											break;
										}
									}
								}

								// �ʘH������ 
								if (bottom < topgate + x)
								{
									for (int bond = topgate + x; bond != bottom; bond--)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}

							}
							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;
						}
					}
					
					
				}

				if (i % GroupWidth == GroupWidth - 1)
				{
					if (i == GroupWidth - 1)
					{
						// ���Ɖ��ɍ쐬
						int bottomgate = (rand() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
						}

						int leftgate = (rand() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].use = true;

						// ���ʘH�̍쐬
						for (int path = x - 1; g_map[leftgate + z][path].Group == i + 1; path--)
						{
							// �E�ʘH�ƌq����
							if (g_map[leftgate + z][path - 1].Group != i + 1)
							{
								// 
								int right;
								// �E�ʘH��z���W��T��
								for (int bz = 0; bz < MAX_MAPHEIGHT; bz++)
								{
									if (g_map[bz][path].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[bz][path].type == 2)
										{
											right = bz;
											break;
										}
									}
								}

								// �ʘH������ 
								if (right < leftgate + z)
								{
									for (int bond = leftgate + z; bond != right; bond--)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].use = true;
						}
					}
					else if (i == GroupHeight * GroupWidth - 1) 
					{
						// ���Ə�ɍ쐬
						int leftgate = (rand() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].use = true;

						// ���ʘH�̍쐬
						for (int path = x - 1; g_map[leftgate + z][path].Group == i + 1; path--)
						{
							// �E�ʘH�ƌq����
							if (g_map[leftgate + z][path - 1].Group != i + 1)
							{
								// 
								int right;
								// �E�ʘH��z���W��T��
								for (int bz = 0; bz < MAX_MAPHEIGHT; bz++)
								{
									if (g_map[bz][path].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[bz][path].type == 2)
										{
											right = bz;
											break;
										}
									}
								}

								// �ʘH������ 
								if (right < leftgate + z)
								{
									for (int bond = leftgate + z; bond != right; bond--)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].use = true;
						}

						int topgate = (rand() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;

						// ��ʘH�̍쐬
						for (int path = z - 1; g_map[path][topgate + x].Group == i + 1; path--)
						{
							// ���ʘH�ƌq����
							if (g_map[path - 1][topgate + x].Group != i + 1)
							{
								// 
								int bottom;
								// ���ʘH��x���W��T��
								for (int bx = 0; bx < MAX_MAPWIDTH; bx++)
								{
									if (g_map[path][bx].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[path][bx].type == 2)
										{
											bottom = bx;
											break;
										}
									}
								}

								// �ʘH������ 
								if (bottom < topgate + x)
								{
									for (int bond = topgate + x; bond != bottom; bond--)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}

							}
							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;
						}
					}
					else
					{
						// ���Ə�Ɖ��ɍ쐬
						int leftgate = (rand() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].use = true;

						// ���ʘH�̍쐬
						for (int path = x - 1; g_map[leftgate + z][path].Group == i + 1; path--)
						{
							// �E�ʘH�ƌq����
							if (g_map[leftgate + z][path - 1].Group != i + 1)
							{
								// 
								int right;
								// �E�ʘH��z���W��T��
								for (int bz = 0; bz < MAX_MAPHEIGHT; bz++)
								{
									if (g_map[bz][path].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[bz][path].type == 2)
										{
											right = bz;
											break;
										}
									}
								}

								// �ʘH������ 
								if (right < leftgate + z)
								{
									for (int bond = leftgate + z; bond != right; bond--)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].use = true;
						}

						int bottomgate = (rand() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
						}

						int topgate = (rand() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;

						// ��ʘH�̍쐬
						for (int path = z - 1; g_map[path][topgate + x].Group == i + 1; path--)
						{
							// ���ʘH�ƌq����
							if (g_map[path - 1][topgate + x].Group != i + 1)
							{
								// 
								int bottom;
								// ���ʘH��x���W��T��
								for (int bx = 0; bx < MAX_MAPWIDTH; bx++)
								{
									if (g_map[path][bx].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[path][bx].type == 2)
										{
											bottom = bx;
											break;
										}
									}
								}

								// �ʘH������ 
								if (bottom < topgate + x)
								{
									for (int bond = topgate + x; bond != bottom; bond--)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}

							}
							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;
						}
					}
				}

				if (i % GroupWidth != 0 && i % GroupWidth != GroupWidth - 1)
				{
					if (i < GroupWidth)
					{
						// �E�ƍ��Ɖ��ɍ쐬
						int leftgate = (rand() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].use = true;

						// ���ʘH�̍쐬
						for (int path = x - 1; g_map[leftgate + z][path].Group == i + 1; path--)
						{
							// �E�ʘH�ƌq����
							if (g_map[leftgate + z][path - 1].Group != i + 1)
							{
								// 
								int right;
								// �E�ʘH��z���W��T��
								for (int bz = 0; bz < MAX_MAPHEIGHT; bz++)
								{
									if (g_map[bz][path].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[bz][path].type == 2)
										{
											right = bz;
											break;
										}
									}
								}

								// �ʘH������ 
								if (right < leftgate + z)
								{
									for (int bond = leftgate + z; bond != right; bond--)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].use = true;
						}

						int bottomgate = (rand() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
						}

						int rightgate = (rand() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}
					}
					else if(i >(GroupHeight - 1) * GroupWidth)
					{
						// �E�ƍ��Ə�ɍ쐬
						int rightgate = (rand() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}

						int topgate = (rand() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;

						// ��ʘH�̍쐬
						for (int path = z - 1; g_map[path][topgate + x].Group == i + 1; path--)
						{
							// ���ʘH�ƌq����
							if (g_map[path - 1][topgate + x].Group != i + 1)
							{
								// 
								int bottom;
								// ���ʘH��x���W��T��
								for (int bx = 0; bx < MAX_MAPWIDTH; bx++)
								{
									if (g_map[path][bx].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[path][bx].type == 2)
										{
											bottom = bx;
											break;
										}
									}
								}

								// �ʘH������ 
								if (bottom < topgate + x)
								{
									for (int bond = topgate + x; bond != bottom; bond--)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}

							}
							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;
						}

						int leftgate = (rand() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].use = true;

						// ���ʘH�̍쐬
						for (int path = x - 1; g_map[leftgate + z][path].Group == i + 1; path--)
						{
							// �E�ʘH�ƌq����
							if (g_map[leftgate + z][path - 1].Group != i + 1)
							{
								// 
								int right;
								// �E�ʘH��z���W��T��
								for (int bz = 0; bz < MAX_MAPHEIGHT; bz++)
								{
									if (g_map[bz][path].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[bz][path].type == 2)
										{
											right = bz;
											break;
										}
									}
								}

								// �ʘH������ 
								if (right < leftgate + z)
								{
									for (int bond = leftgate + z; bond != right; bond--)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].use = true;
						}
					}
					else
					{
						// �S�����ɍ쐬
						int rightgate = (rand() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}

						int topgate = (rand() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;

						// ��ʘH�̍쐬
						for (int path = z - 1; g_map[path][topgate + x].Group == i + 1; path--)
						{
							// ���ʘH�ƌq����
							if (g_map[path - 1][topgate + x].Group != i + 1)
							{
								// 
								int bottom;
								// ���ʘH��x���W��T��
								for (int bx = 0; bx < MAX_MAPWIDTH; bx++)
								{
									if (g_map[path][bx].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[path][bx].type == 2)
										{
											bottom = bx;
											break;
										}
									}
								}

								// �ʘH������ 
								if (bottom < topgate + x)
								{
									for (int bond = topgate + x; bond != bottom; bond--)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
									}
								}

							}
							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;
						}

						int leftgate = (rand() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].use = true;

						// ���ʘH�̍쐬
						for (int path = x - 1; g_map[leftgate + z][path].Group == i + 1; path--)
						{
							// �E�ʘH�ƌq����
							if (g_map[leftgate + z][path - 1].Group != i + 1)
							{
								// 
								int right;
								// �E�ʘH��z���W��T��
								for (int bz = 0; bz < MAX_MAPHEIGHT; bz++)
								{
									if (g_map[bz][path].Group != i + 1)
									{
										continue;
									}
									else
									{
										if (g_map[bz][path].type == 2)
										{
											right = bz;
											break;
										}
									}
								}

								// �ʘH������ 
								if (right < leftgate + z)
								{
									for (int bond = leftgate + z; bond != right; bond--)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].use = true;
						}

						int bottomgate = (rand() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
						}
					}
				}

				


			}

			
		}

		
	}

	
	

	
	// ����̎�ނ����߂�
	// �f�o�b�O�p�t���A
	/*for (int z = 1; z <= 5; z++)
	{
		for (int x = 1; x <= 9; x++)
		{
			g_map[z][x].type = 1;
			g_map[z][x].use = true;
		}
	}*/

	// �f�o�b�O�p�ʘH
	
	/*for (int x = 2; x <= 8; x++)
	{
		g_map[98][x].type = 2;
		g_map[98][x].use = true;
	}
	for (int z = 93; z <= 97; z++)
	{
		g_map[z][2].type = 2;
		g_map[z][2].use = true;
	}*/

	
	/*
	// ����̎�ނ����߂�
	for (int z = 25; z <= 31; z++)
	{
		for (int x = 55; x <= 60; x++)
		{
			g_map[z][x].type = 1;
			g_map[z][x].use = true;
		}
	}
	*/


	// ���߂�����ɉ����ĕǂ�z�u
	for (int z = 0; z < MAX_MAPHEIGHT; z++)
	{
		for (int x = 0; x < MAX_MAPWIDTH; x++)
		{

			//===================================================
			// �t���A
			//===================================================
			if (g_map[z][x].type == 1)
			{
				
				//===================================================
				// �t���A��Ǎ쐬
				//===================================================
				if (!g_map[z][x].Cxwall&&g_map[z - 1][x].type == 0)
				{
					int Xwallwidth = 0;
					for (int Xwall = 0; g_map[z][x + Xwall].type == 1; Xwall++)
					{
						g_map[z][x + Xwall].Cxwall = true;
						Xwallwidth++;
					}
					//MeshWall_Create(g_TexWood, 50, 10, 50, 10, D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//���
				//						    1�}�X*5											// -220
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Xwallwidth * 5, 10, Xwallwidth * 5, 10, D3DXVECTOR3((g_map[z][x].pos.x + g_map[z][x + Xwallwidth - 1].pos.x) / 2, 0.0f, g_map[z][x].pos.z + 2.5f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//���
				}
				
				//===================================================
				// �t���A���Ǎ쐬
				//===================================================
				if (!g_map[z][x].Cxwall&&g_map[z + 1][x].type == 0)
				{
					int Xwallwidth = 0;
					for (int Xwall = 0; g_map[z][x + Xwall].type == 1; Xwall++)
					{
						g_map[z][x + Xwall].Cxwall = true;
						Xwallwidth++;
					}
					//MeshWall_Create(g_TexWood, 50, 10, 50, 10, D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//����
				//						    1�}�X*5											// -220
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Xwallwidth * 5, 10, Xwallwidth * 5, 10, D3DXVECTOR3((g_map[z][x].pos.x + g_map[z][x + Xwallwidth - 1].pos.x) / 2, 0.0f, g_map[z][x].pos.z - 2.5f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//����
				}

				//===================================================
				// �t���A���Ǎ쐬
				//===================================================
				if (!g_map[z][x].Czwall&&g_map[z][x - 1].type == 0)
				{
					int Zwallwidth = 0;
					for (int Zwall = 0; g_map[z + Zwall][x].type == 1; Zwall++)
					{
						g_map[z + Zwall][x].Czwall = true;
						Zwallwidth++;
					}
					//MeshWall_Create(g_TexWood, 50, 10, 50, 10, D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//����
				//						    1�}�X*5											// -220
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Zwallwidth * 5, 10, Zwallwidth * 5, 10, D3DXVECTOR3(g_map[z][x].pos.x - 2.5f, 0.0f, (g_map[z][x].pos.z + g_map[z + Zwallwidth - 1][x].pos.z) / 2), D3DXVECTOR3(1.0f, 0.0f, 0.0f));		//����
				}

				//===================================================
				// �t���A�E�Ǎ쐬
				//===================================================
				if (!g_map[z][x].Czwall&&g_map[z][x + 1].type == 0)
				{
					int Zwallwidth = 0;
					for (int Zwall = 0; g_map[z + Zwall][x].type == 1; Zwall++)
					{
						g_map[z + Zwall][x].Czwall = true;
						Zwallwidth++;
					}
					//MeshWall_Create(g_TexWood, 50, 10, 50, 10, D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//�E��
				//						    1�}�X*5											// -220
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Zwallwidth * 5, 10, Zwallwidth * 5, 10, D3DXVECTOR3(g_map[z][x].pos.x + 2.5f, 0.0f, (g_map[z][x].pos.z + g_map[z + Zwallwidth - 1][x].pos.z) / 2), D3DXVECTOR3(-1.0f, 0.0f, 0.0f));		//�E��
				}
			}

			//===================================================
			// �ʘH
			//===================================================
			if (g_map[z][x].type == 2)
			{
				//===================================================
				// �ʘH��Ǎ쐬
				//===================================================
				if (!g_map[z][x].Cxtopwall&&g_map[z - 1][x].type == 0)
				{
					int Xwallwidth = 0;
					for (int Xwall = 0; g_map[z][x + Xwall].type == 2; Xwall++)
					{
						if (!g_map[z - 1][x + Xwall].type == 0)
						{
							continue;
						}
						g_map[z][x + Xwall].Cxtopwall = true;
						Xwallwidth++;
					}
					//MeshWall_Create(g_TexWood, 50, 10, 50, 10, D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//���
				//						    1�}�X*5											// -220
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Xwallwidth * 5, 10, Xwallwidth * 5, 10, D3DXVECTOR3((g_map[z][x].pos.x + g_map[z][x + Xwallwidth - 1].pos.x) / 2, 0.0f, g_map[z][x].pos.z + 2.5f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//���
				}

				//===================================================
				// �ʘH���Ǎ쐬
				//===================================================
				if (!g_map[z][x].Cxbotwall&&g_map[z + 1][x].type == 0)
				{
					int Xwallwidth = 0;
					for (int Xwall = 0; g_map[z][x + Xwall].type == 2; Xwall++)
					{
						if (!g_map[z + 1][x + Xwall].type == 0)
						{
							continue;
						}
						g_map[z][x + Xwall].Cxbotwall = true;
						Xwallwidth++;
					}
					//MeshWall_Create(g_TexWood, 50, 10, 50, 10, D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//����
				//						    1�}�X*5											// -220
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Xwallwidth * 5, 10, Xwallwidth * 5, 10, D3DXVECTOR3((g_map[z][x].pos.x + g_map[z][x + Xwallwidth - 1].pos.x) / 2, 0.0f, g_map[z][x].pos.z - 2.5f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//����
				}

				//===================================================
				// �ʘH���Ǎ쐬
				//===================================================
				if (!g_map[z][x].CzLwall&&g_map[z][x - 1].type == 0)
				{
					int Zwallwidth = 0;
					for (int Zwall = 0; g_map[z + Zwall][x].type == 2; Zwall++)
					{
						if (!g_map[z + Zwall][x - 1].type == 0)
						{
							continue;
						}
						g_map[z + Zwall][x].CzLwall = true;
						Zwallwidth++;
					}
					//MeshWall_Create(g_TexWood, 50, 10, 50, 10, D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//����
				//						    1�}�X*5											// -220
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Zwallwidth * 5, 10, Zwallwidth * 5, 10, D3DXVECTOR3(g_map[z][x].pos.x - 2.5f, 0.0f, (g_map[z][x].pos.z + g_map[z + Zwallwidth - 1][x].pos.z) / 2), D3DXVECTOR3(1.0f, 0.0f, 0.0f));		//����
				}

				//===================================================
				// �ʘH�E�Ǎ쐬
				//===================================================
				if (!g_map[z][x].CzRwall&&g_map[z][x + 1].type == 0)
				{
					int Zwallwidth = 0;
					for (int Zwall = 0; g_map[z + Zwall][x].type == 2; Zwall++)
					{
						if (!g_map[z + Zwall][x + 1].type == 0)
						{
							continue;
						}
						g_map[z + Zwall][x].CzRwall = true;
						Zwallwidth++;
					}
					//MeshWall_Create(g_TexWood, 50, 10, 50, 10, D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//�E��
				//						    1�}�X*5											// -220
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Zwallwidth * 5, 10, Zwallwidth * 5, 10, D3DXVECTOR3(g_map[z][x].pos.x + 2.5f, 0.0f, (g_map[z][x].pos.z + g_map[z + Zwallwidth - 1][x].pos.z) / 2), D3DXVECTOR3(-1.0f, 0.0f, 0.0f));		//�E��
				}
			}

			
			if (g_map[z][x].type == 3)
			{
				//===================================================
				// �p�΍��Ǎ쐬
				//===================================================
				if (!g_map[z][x].Cxtopwall&&g_map[z - 1][x].type == 0)
				{
					int Xwallwidth = 0;
					g_map[z][x].Cxtopwall = true;
					Xwallwidth++;
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Xwallwidth * 5, 10, Xwallwidth * 5, 10, D3DXVECTOR3((g_map[z][x].pos.x + g_map[z][x + Xwallwidth - 1].pos.x) / 2, 0.0f, g_map[z][x].pos.z + 2.5f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//���
				}

				//===================================================
				// �p�΍􉺕Ǎ쐬
				//===================================================
				if (!g_map[z][x].Cxbotwall&&g_map[z + 1][x].type == 0)
				{
					int Xwallwidth = 0;
					g_map[z][x].Cxbotwall = true;
					Xwallwidth++;
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Xwallwidth * 5, 10, Xwallwidth * 5, 10, D3DXVECTOR3((g_map[z][x].pos.x + g_map[z][x + Xwallwidth - 1].pos.x) / 2, 0.0f, g_map[z][x].pos.z - 2.5f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));		//����
				}

				//===================================================
				// �p�΍􍶕Ǎ쐬
				//===================================================
				if (!g_map[z][x].CzLwall&&g_map[z][x - 1].type == 0)
				{
					int Zwallwidth = 0;
					g_map[z][x].CzLwall = true;
					Zwallwidth++;
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Zwallwidth * 5, 10, Zwallwidth * 5, 10, D3DXVECTOR3(g_map[z][x].pos.x - 2.5f, 0.0f, (g_map[z][x].pos.z + g_map[z + Zwallwidth - 1][x].pos.z) / 2), D3DXVECTOR3(1.0f, 0.0f, 0.0f));		//����
				}

				//===================================================
				// �p�΍�E�Ǎ쐬
				//===================================================
				if (!g_map[z][x].CzRwall&&g_map[z][x + 1].type == 0)
				{
					int Zwallwidth = 0;
					g_map[z][x].CzRwall = true;
					Zwallwidth++;
					CWall::MeshWall_Create(CTexture::TEX_WOOD, Zwallwidth * 5, 10, Zwallwidth * 5, 10, D3DXVECTOR3(g_map[z][x].pos.x + 2.5f, 0.0f, (g_map[z][x].pos.z + g_map[z + Zwallwidth - 1][x].pos.z) / 2), D3DXVECTOR3(-1.0f, 0.0f, 0.0f));		//�E��
				}
			}

		}
	}

	//===================================================
	// ��q�𐶐�
	//===================================================
	int lposX;
	int lposZ;
	do
	{
		lposX = rand() % 100;
		lposZ = rand() % 100;
	} while (g_map[lposZ][lposX].type != 1);
	//g_map[pposZ][pposX].have = HAVEPLAYER;
	CObject::Create(CObject::TYPE_LADDER, lposX, lposZ);;

	//===================================================
	// �v���C���[�̏����ʒu�����߂�
	//===================================================
	int pposX;
	int pposZ;
	do
	{
		pposX = rand() % 100;
		pposZ = rand() % 100;
	} while (g_map[pposZ][pposX].type != 1);
	g_map[pposZ][pposX].have = HAVEPLAYER;
	CPlayer::Player_SetPos(pposZ, pposX);

	//===================================================
	// �G�𐶐� GroupWidth * GroupHeight - 0�`2�̐���
	//===================================================
	// �G�������̌덷
	//int setenemy = rand() % 3;
	int setenemy = 0;
	for (int i = 0; i < 1; i++)
	//for (int i = 0; i < 1; i++)	// �f�o�b�O�p
	{
		int eposX;
		int eposZ;
		do
		{
			eposX = rand() % MAX_MAPWIDTH;
			eposZ = rand() % MAX_MAPHEIGHT;
		} while (g_map[eposZ][eposX].type != 1 || g_map[eposZ][eposX].have != NOTHAVE);
		g_map[eposZ][eposX].have = HAVEENEMY;
		CEnemy::Create(CEnemy::TYPE_SRIME,eposX, eposZ);
	}
	/*do
	{
		eposX = rand() % MAX_MAPWIDTH;
		eposZ = rand() % MAX_MAPHEIGHT;
	} while (g_map[eposZ][eposX].type != 1 || g_map[eposZ][eposX].have != NOTHAVE);
	g_map[eposZ][eposX].have = HAVEENEMY;
	Enemy_Create(eposZ, eposX);*/
	// �������̉��
	//free(base_g_map);
	//free(g_map);
}

void CMap::Map_Finalize(void)
{
	// �������̉��
	free(base_g_map);
	free(g_map);
}

void CMap::Map_Draw(void)
{

}
