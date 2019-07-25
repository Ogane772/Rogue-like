#include "map.h"
#include "wall.h"
#include "meshfield.h"
#include "CTexture.h"
#include "Cplayer.h"
#include "CEnemy.h"
#include <malloc.h>
#include <memory.h>
#include "CObject.h"
#include "CTrap.h"
#include "stage.h"
#include "CWepon.h"
#include <iostream>
#include <random>
int CMap::GroupWidth;
int CMap::GroupHeight;
int CMap::DeletePassageNum;
//int CMap::g_TexWood2;
CMap::MAP **CMap::g_map, *CMap::base_g_map;

CMap::DELETEPASSAGE *CMap::g_deletepassage;
void CMap::Map_Initialize(void)
{
	CEnemy::DeleteAllEnemy();
	CObject::DeleteAllObject();
	CWepon::DeleteAllWepon();
	CTrap::DeleteAllTrap();
	int i = 0, j = 0;
	//g_TexWood2 = -1;
	if (CStage::Stage_GetLevel() == 1)
	{
		GroupWidth = 3;
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

	DeletePassageNum = GroupHeight * GroupWidth / 3;
	for (i = 0; i < MAX_MAPHEIGHT; i++)
	{
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
			g_map[z][x].Cellingwall = false;
			g_map[z][x].Cxtopwall = false;
			g_map[z][x].Cxbotwall = false;
			g_map[z][x].CzRwall = false;
			g_map[z][x].CzLwall = false;
			g_map[z][x].pos = D3DXVECTOR3(-247.5f, 0.0f, 247.5f);
			g_map[z][x].pos.x += x * 5.0f;
			g_map[z][x].pos.z += -z * 5.0f;
			g_map[z][x].passageNum = 0;
			//g_map[z][x].Group = x / GroupWidth;
			g_map[z][x].Group = 0;	// �O���[�v�t��

		}
	}
	for (i = 0; i < MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight; i++)
	{

		for (j = 0; j < MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth; j++)
		{
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].type = 0;		// ���ꖳ��
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].have = NOTHAVE;
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].use = false;
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].Cxwall = false;
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].Czwall = false;
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].Cxtopwall = false;
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].Cxbotwall = false;
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].CzRwall = false;
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].CzLwall = false;
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].pos = D3DXVECTOR3(-247.5f, 0.0f, 247.5f);
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].pos.x += (j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2) * 5.0f;
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].pos.z += (i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2) * -5.0f;
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].alpha = 0;
			//g_map[z][x].Group = x / GroupWidth;
			g_map[i + (MAX_MAPHEIGHT - MAX_MAPHEIGHT / MAP_AXIS_MAX * GroupHeight) / 2][j + (MAX_MAPWIDTH - MAX_MAPWIDTH / MAP_AXIS_MAX * GroupWidth) / 2].Group = (i / (MAX_MAPWIDTH / MAP_AXIS_MAX) * GroupWidth) + (j / (MAX_MAPWIDTH / MAP_AXIS_MAX) + 1);	// �O���[�v�t��

		}
	}
}


void CMap::Map_Create(void)
{

	//===================================================
	// �t���A��GroupHeight * GroupWidth����
	//===================================================
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 99);

	std::uniform_int_distribution<int> floorsize(4, 13);
	int i;
	int h, w;
	for (i = 0; i < GroupHeight * GroupWidth; i++)
	{


		// �t���A�n�_z��
		int z = mt() % (MAX_MAPHEIGHT / MAP_AXIS_MAX * (i / GroupWidth + 1));
		// �����̂�蒼���`�F�b�N
		if (z <= 4 + MAX_MAPHEIGHT / MAP_AXIS_MAX * (i / GroupWidth) ||
			z >= MAX_MAPHEIGHT / MAP_AXIS_MAX * (i / GroupWidth + 1) - 14)
		{
			i--;
			continue;
		}
		else
		{

			// �t���A�n�_x�� 
			// �~�X
			int x = mt() % (MAX_MAPWIDTH / MAP_AXIS_MAX * (i % GroupWidth + 1));
			// �����̂�蒼���`�F�b�N
			if (x <= 4 + (MAX_MAPWIDTH / MAP_AXIS_MAX * (i % GroupWidth)) ||
				x >= MAX_MAPWIDTH / MAP_AXIS_MAX * (i % GroupWidth + 1) - 14)
			{
				i--;
				continue;
			}
			else
			{
				// �����Ƃ̃t���A���ɂ���ăt���A�̈ʒu�𒲐�
				if (GroupHeight < MAP_AXIS_MAX)
					z += MAX_MAPHEIGHT / MAP_AXIS_MAX / (GroupHeight - 1);
				if (GroupWidth < MAP_AXIS_MAX)
					x += MAX_MAPWIDTH / MAP_AXIS_MAX / (GroupWidth - 1);


				// �t���A�̏c�̒���4�`13
				int height = floorsize(mt) + z;
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);

				// �t���A�̉��̒���4�`13
				int width = floorsize(mt) + x;
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);
				//while (z <= height)
				for (h = z; h <= height; h++)
				{


					//while (x <= width)
					// �t���A�쐬
					for (w = x; w <= width; w++)
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
						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}

						int bottomgate = (mt() % (width - x - 1)) + 1;
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

						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}

						int topgate = (mt() % (width - x - 1)) + 1;
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
					// �E�Ə�Ɖ��ɍ쐬
					else
					{
						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}

						int bottomgate = (mt() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
						}

						int topgate = (mt() % (width - x - 1)) + 1;
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
						int bottomgate = (mt() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
						}

						int leftgate = (mt() % (height - z - 1)) + 1;
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
						int leftgate = (mt() % (height - z - 1)) + 1;
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

						int topgate = (mt() % (width - x - 1)) + 1;
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
						int leftgate = (mt() % (height - z - 1)) + 1;
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

						int bottomgate = (mt() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
						}

						int topgate = (mt() % (width - x - 1)) + 1;
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
						int leftgate = (mt() % (height - z - 1)) + 1;
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

						int bottomgate = (mt() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
						}

						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}
					}
					else if (i >(GroupHeight - 1) * GroupWidth)
					{
						// �E�ƍ��Ə�ɍ쐬
						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}

						int topgate = (mt() % (width - x - 1)) + 1;
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

						int leftgate = (mt() % (height - z - 1)) + 1;
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
						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}

						int topgate = (mt() % (width - x - 1)) + 1;
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

						int leftgate = (mt() % (height - z - 1)) + 1;
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

						int bottomgate = (mt() % (width - x - 1)) + 1;
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
	// ���߂�����ɉ����ĕǂ�z�u
	//===================================================
	// �ǂ𐶐�
	//===================================================
	MapWallSet();
	//===================================================
	// ��q�𐶐�
	//===================================================
	MapLadderSet();
	//===================================================
	// �A�C�e���𐶐�
	//===================================================
	MapItemSet();
	//===================================================
	// �����𐶐�
	//===================================================
	MapWeponSet();
	//===================================================
	// ���i�𐶐�
	//===================================================
	MapTrapSet();
	//===================================================
	// �v���C���[�̏����ʒu�����߂�
	//===================================================
	MapPlayerSet();
	//===================================================
	// �G�𐶐� GroupWidth * GroupHeight - 0�`2�̐���
	//===================================================
	MapEnemySet();
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

void CMap::MapItemSet(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 99);
	//int setitem = mt() % 3 + 4;
	int setitem = 5;
	int itemdrop_number[100] = { 0 };//�A�C�e���h���b�v���i�[
	int kakuritu_start = 0;//�ǂ̔z��ԍ����琔���邩
	int lposX;
	int lposZ;
	int i, j, k;
	//���̊K�ŏo��A�C�e�����������m���������Ă���
	for (j = 0; j<CObject::Get_ITEMDATAMAX(); j++)
	{
		if (CObject::Get_Item_Data(j)->first_floor <= CStage::Stage_GetLevel() &&
			CObject::Get_Item_Data(j)->end_floor >= CStage::Stage_GetLevel())
		{
			for (k = kakuritu_start; k < kakuritu_start + CObject::Get_Item_Data(j)->itemchance; k++)
			{
				if (itemdrop_number[k] == 0)
				{
					itemdrop_number[k] = CObject::Get_Item_Data(j)->object_type;
				}
			}
			kakuritu_start += CObject::Get_Item_Data(j)->itemchance;
		}
	}
	//������100�����܂��ĂȂ�������󂫂ɎG����u���Ă���
	for (k = 0; k < 100; k++)
	{
		if (itemdrop_number[k] == 0)
		{
			itemdrop_number[k] = CObject::TYPE_ZAXTUSOU;
		}
	}
	//�����Ŏ擾�����������A�C�e����z�u����
	for (i = 0; i < setitem; i++)
	{
		for (;;)
		{
			lposX = random(mt);
			lposZ = random(mt);
			if (g_map[lposZ][lposX].type == 1 && g_map[lposZ][lposX].have == NOTHAVE)
				break;
		}
		CObject::Create(itemdrop_number[random(mt)], lposX, lposZ);
		g_map[lposZ][lposX].have = HAVEITEM;
	}
}

void CMap::MapItemPosSet(int item_type, int x, int z)
{
	CObject::Create(item_type, x, z);
	g_map[x][z].have = HAVEITEM;
}

void CMap::MapWeponSet(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 99);
	std::uniform_int_distribution<int> randomwepon(1, 8);
	//int setitem = mt() % 3 + 4;
	int setitem = 5;
	int lposX;
	int lposZ;
	int i;

	for (i = 0; i < setitem; i++)
	{
		for (;;)
		{
			lposX = random(mt);
			lposZ = random(mt);
			if (g_map[lposZ][lposX].type == 1 && g_map[lposZ][lposX].have == NOTHAVE)
				break;
		}
		CWepon::Create(randomwepon(mt), lposX, lposZ);
		g_map[lposZ][lposX].have = HAVEWEPON;
	}
}

void CMap::MapTrapSet(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 99);
	int settrap = mt() % 3 + 4;
	//int settrap = 40;
	int trapdrop_number[100] = { 0 };//���i�ݒu���i�[
	int kakuritu_start = 0;//�ǂ̔z��ԍ����琔���邩
	int lposX;
	int lposZ;
	int i, j, k;
	//���̊K�ŏo��A�C�e�����������m���������Ă���
	for (j = 0; j<CTrap::Get_TRAPDATAMAX(); j++)
	{
		if (CTrap::Get_Trap_Data(j)->first_floor <= CStage::Stage_GetLevel() &&
			CTrap::Get_Trap_Data(j)->end_floor >= CStage::Stage_GetLevel())
		{
			for (k = kakuritu_start; k < kakuritu_start + CTrap::Get_Trap_Data(j)->trapchance; k++)
			{
				if (trapdrop_number[k] == 0)
				{
					trapdrop_number[k] = CTrap::Get_Trap_Data(j)->trap_type;
				}
			}
			kakuritu_start += CTrap::Get_Trap_Data(j)->trapchance;
		}
	}
	//������100�����܂��ĂȂ�������󂫂Ƀ_���[�W����u���Ă���
	for (k = 0; k < 100; k++)
	{
		if (trapdrop_number[k] == 0)
		{
			trapdrop_number[k] = CTrap::TYPE_DAMAGE;
		}
	}
	//�����Ŏ擾�����������A�C�e����z�u����
	for (i = 0; i < settrap; i++)
	{
		for (;;)
		{
			lposX = random(mt);
			lposZ = random(mt);
			if (g_map[lposZ][lposX].type == 1 && g_map[lposZ][lposX].have == NOTHAVE)
				break;
		}
		CTrap::Create(trapdrop_number[random(mt)], lposX, lposZ);
		g_map[lposZ][lposX].have = HAVETRAP;
	}
}

void CMap::Map_CreateOneFloor(void)
{
	for (int z = 47; z < 53; z++)
	{

		for (int x = 47; x < 53; x++)
		{
			g_map[z][x].type = 1;
			g_map[z][x].use = false;
		}
	}



	MapWallSet();
	//MapLadderSet();
	//MapItemSet();
	//MapWeponSet();
	//MapEnemySet();
	MapPlayerSet();

}

void CMap::MapPlayerSet(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());

	int pposX;
	int pposZ;
	std::uniform_int_distribution<int> random(0, 99);
	for (;;)
	{
		pposX = random(mt);
		pposZ = random(mt);
		if (g_map[pposZ][pposX].type == 1 && g_map[pposZ][pposX].have == NOTHAVE)
			break;
	}
	g_map[pposZ][pposX].have = HAVEPLAYER;
	CPlayer::Player_SetPos(pposZ, pposX);
	//CPlayer::Player_SetPos(50, 50);
}

void CMap::WorpPlayerSet(int z, int x)
{
	std::random_device rd;
	std::mt19937 mt(rd());

	int pposX;
	int pposZ;
	std::uniform_int_distribution<int> random(0, 99);
	for (;;)
	{
		pposX = random(mt);
		pposZ = random(mt);
		if (g_map[pposZ][pposX].type == 1 && g_map[pposZ][pposX].have == NOTHAVE &&
			g_map[pposZ][pposX].Group != g_map[z][x].Group &&
			g_map[pposZ][pposX].have != HAVEENEMY)
			break;
	}
	g_map[pposZ][pposX].have = HAVEPLAYER;
	CPlayer::Player_SetWorpPos(pposZ, pposX);
}

void CMap::WorpEnemySet(C3DObj *enemy, int z, int x)
{
	std::random_device rd;
	std::mt19937 mt(rd());

	int pposX;
	int pposZ;
	std::uniform_int_distribution<int> random(0, 99);
	for (;;)
	{
		pposX = random(mt);
		pposZ = random(mt);
		if (g_map[pposZ][pposX].type == 1 && g_map[pposZ][pposX].have == NOTHAVE &&
			g_map[pposZ][pposX].Group != g_map[z][x].Group)
			break;
	}
	g_map[pposZ][pposX].have = HAVEENEMY;
	enemy->Enemy_SetWorpPos(pposZ, pposX);
}

void CMap::MapEnemySet(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 99);
	int setenemy = 0;
	int enemysummon_number[100] = { 0 };//�G�l�~�[�o�����i�[
	int kakuritu_start = 0;//�ǂ̔z��ԍ����琔���邩
	int lposX;
	int lposZ;
	int i, j, k;
	//���̊K�ŏo��A�C�e�����������m���������Ă���
	for (j = 0; j< CEnemy::Get_ENEMYDATAMAX(); j++)
	{
		if (CEnemy::Get_EnemyData(j)->first_floor <= CStage::Stage_GetLevel() &&
			CEnemy::Get_EnemyData(j)->end_floor >= CStage::Stage_GetLevel())
		{
			for (k = kakuritu_start; k < kakuritu_start + CEnemy::Get_EnemyData(j)->enemychance; k++)
			{
				if (enemysummon_number[k] == 0)
				{
					enemysummon_number[k] = CEnemy::Get_EnemyData(j)->enemy_type;
				}
			}
			kakuritu_start += CEnemy::Get_EnemyData(j)->enemychance;
		}
	}
	//������100�����܂��ĂȂ�������󂫂ɍŏ��̃G�l�~�[�����Ă���
	for (k = 0; k < 100; k++)
	{
		if (enemysummon_number[k] == 0)
		{
			enemysummon_number[k] = enemysummon_number[0];
		}
	}
	//�����Ŏ擾�����������G�l�~�[����z�u����
	for (i = 0; i < setenemy; i++)
	{
		for (;;)
		{
			lposX = random(mt);
			lposZ = random(mt);
			if (g_map[lposZ][lposX].type == 1 && g_map[lposZ][lposX].have == NOTHAVE)
				break;
		}
		CEnemy::Create(enemysummon_number[random(mt)], lposX, lposZ);
		g_map[lposZ][lposX].have = HAVEENEMY;
	}
}

void CMap::MapLadderSet(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());

	int lposX;
	int lposZ;
	std::uniform_int_distribution<int> random(0, 99);
	for (;;)
	{
		lposX = random(mt);
		lposZ = random(mt);
		if (g_map[lposZ][lposX].type == 1 && g_map[lposZ][lposX].have == NOTHAVE)
			break;
	}
	g_map[lposZ][lposX].have = HAVELADDER;
	CObject::Create(CObject::TYPE_LADDER, lposX, lposZ);
}

void CMap::MapWallSet(void)
{
	for (int z = 0; z < MAX_MAPHEIGHT; z++)
	{
		for (int x = 0; x < MAX_MAPWIDTH; x++)
		{
			//===================================================
			// �V��쐬
			//===================================================
			//if (g_map[z][x].type == 0 && !g_map[z][x].Cellingwall)
			//{
			//	int celingwidth = 0;
			//	int celingheightcount;
			//	int celingheight = 0;
			//	for (int Xwall = 0; Xwall + x  < MAX_MAPWIDTH && g_map[z][x + Xwall].type == 0; Xwall++)
			//	{
			//		celingwidth++;
			//		
			//		// ��������o�^
			//		if (celingwidth == 1)
			//		{
			//			for (int Zwall = 0;Zwall + z < MAX_MAPHEIGHT && g_map[z + Zwall][x + Xwall].type == 0 && !g_map[z + Zwall][x + Xwall].Cellingwall; Zwall++)
			//			{
			//				g_map[z + Zwall][x + Xwall].Cellingwall = true;
			//				
			//				celingheight++;
			//			}
			//		}
			//		
			//		// �o�^�����������ƈꏏ�Ȃ牡�����Z
			//		if (celingwidth > 1)
			//		{
			//			int delz = 0;
			//			celingheightcount = 0;
			//			for (int Zwall = 0;Zwall + z < MAX_MAPHEIGHT && g_map[z + Zwall][x + Xwall].type == 0 && !g_map[z + Zwall][x + Xwall].Cellingwall; Zwall++)
			//			{
			//				g_map[z + Zwall][x + Xwall].Cellingwall = true;
	
			//				celingheightcount++;
			//				delz++;		// �����ꍇCelingwall��false�ɖ߂�
			//				if (celingheight == celingheightcount)
			//					break;
			//			}
			//			if (celingheight > celingheightcount)
			//			{
			//				for (int dz = 0; dz <= delz; dz++)
			//				{
			//					g_map[z + dz][x + Xwall].Cellingwall = false;
			//				}
			//				celingwidth -= 1;
			//				//Xwall -= 1;
			//				
			//				break;
			//			}
			//				
			//		}


			//	}
			//	CMeshField::MeshField_Create(CTexture::TEX_BLACKUP, celingwidth * 5, celingheight * 5, celingwidth, celingheight, D3DXVECTOR3((g_map[z][x].pos.x + g_map[z + celingheight - 1][x + celingwidth - 1].pos.x) / 2, 5.0f, (g_map[z][x].pos.z + g_map[z + celingheight - 1][x + celingwidth - 1].pos.z) / 2));	// 1��  = 5.0f * 5.0f
			//
			//}

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
}


void CMap::Map_Create_B(void)
{
	//===================================================
	// �t���A��GroupHeight * GroupWidth����
	//===================================================
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 99);

	std::uniform_int_distribution<int> floorsize(4, 13);
	int i;
	int h, w;




	g_deletepassage = (DELETEPASSAGE*)malloc(sizeof(DELETEPASSAGE) * (GroupHeight * GroupWidth) / 3);
	for (i = 0; i < GroupHeight * GroupWidth; i++)
	{

		// �t���A�n�_z��
		int z = mt() % (MAX_MAPHEIGHT / MAP_AXIS_MAX * (i / GroupWidth + 1));
		// �����̂�蒼���`�F�b�N
		if (z <= 4 + MAX_MAPHEIGHT / MAP_AXIS_MAX * (i / GroupWidth) ||
			z >= MAX_MAPHEIGHT / MAP_AXIS_MAX * (i / GroupWidth + 1) - 14)
		{
			i--;
			continue;
		}
		else
		{

			// �t���A�n�_x�� 
			// �~�X
			int x = mt() % (MAX_MAPWIDTH / MAP_AXIS_MAX * (i % GroupWidth + 1));
			// �����̂�蒼���`�F�b�N
			if (x <= 4 + (MAX_MAPWIDTH / MAP_AXIS_MAX * (i % GroupWidth)) ||
				x >= MAX_MAPWIDTH / MAP_AXIS_MAX * (i % GroupWidth + 1) - 14)
			{
				i--;
				continue;
			}
			else
			{
				// �����Ƃ̃t���A���ɂ���ăt���A�̈ʒu�𒲐�
				if (GroupHeight < MAP_AXIS_MAX)
					z += MAX_MAPHEIGHT / MAP_AXIS_MAX / (GroupHeight - 1);
				if (GroupWidth < MAP_AXIS_MAX)
					x += MAX_MAPWIDTH / MAP_AXIS_MAX / (GroupWidth - 1);


				// �t���A�̏c�̒���4�`13
				int height = floorsize(mt) + z;
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);

				// �t���A�̉��̒���4�`13
				int width = floorsize(mt) + x;
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);
				floorsize(mt);
				//while (z <= height)
				for (h = z; h <= height; h++)
				{


					//while (x <= width)
					// �t���A�쐬
					for (w = x; w <= width; w++)
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
						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].passageNum = 1;
						g_map[rightgate + z][width].use = true;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].passageNum = 1;
							g_map[rightgate + z][path].use = true;
						}

						int bottomgate = (mt() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].passageNum = GroupWidth;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].passageNum = GroupWidth;
							g_map[path][bottomgate + x].use = true;
						}
					}
					// �E�Ə�ɍ쐬
					else if (i + GroupWidth == GroupHeight * GroupWidth)
					{

						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;

						g_map[rightgate + z][width].use = true;
						g_map[rightgate + z][width].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + 1;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + 1;
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
						}

						int topgate = (mt() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;
						g_map[z][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth;

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
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth;
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth;
									}
								}

							}

							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;
							g_map[path][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth;

						}


					}
					// �E�Ə�Ɖ��ɍ쐬
					else
					{
						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;
						g_map[rightgate + z][width].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + 1;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
							g_map[rightgate + z][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + 1;
						}

						int bottomgate = (mt() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;
						g_map[height][bottomgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + GroupWidth;
						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
							g_map[path][bottomgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + GroupWidth;
						}

						int topgate = (mt() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;
						g_map[z][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth;

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
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth;
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth;
									}
								}

							}
							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;
							g_map[path][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth;
						}
					}


				}

				if (i % GroupWidth == GroupWidth - 1)
				{
					if (i == GroupWidth - 1)
					{
						// ���Ɖ��ɍ쐬
						int bottomgate = (mt() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].passageNum = i + GroupWidth;
						g_map[height][bottomgate + x].use = true;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].passageNum = i + GroupWidth;
							g_map[path][bottomgate + x].use = true;
						}

						int leftgate = (mt() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].passageNum = i;
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
										g_map[bond][path].passageNum = i;
										g_map[bond][path].use = true;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].passageNum = i;
										g_map[bond][path].use = true;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].passageNum = i;
							g_map[leftgate + z][path].use = true;
						}
					}
					else if (i == GroupHeight * GroupWidth - 1)
					{
						// ���Ə�ɍ쐬
						int leftgate = (mt() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].use = true;
						g_map[leftgate + z][x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + GroupWidth - 1;
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
										g_map[bond][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + GroupWidth - 1;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
										g_map[bond][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + GroupWidth - 1;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].use = true;
							g_map[leftgate + z][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + GroupWidth - 1;
						}

						int topgate = (mt() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;
						g_map[z][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth);

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
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth);
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth);
									}
								}

							}
							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;
							g_map[path][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth);
						}
					}
					else
					{
						// ���Ə�Ɖ��ɍ쐬
						int leftgate = (mt() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + GroupWidth - 1;
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
										g_map[bond][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + GroupWidth - 1;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
										g_map[bond][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + GroupWidth - 1;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].use = true;
							g_map[leftgate + z][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + GroupWidth - 1;
						}

						int bottomgate = (mt() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;
						g_map[height][bottomgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth + 1);

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
							g_map[path][bottomgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth + 1);
						}

						int topgate = (mt() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;
						g_map[z][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth);
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
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth);
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth);
									}
								}

							}
							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;
							g_map[path][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth);
						}
					}
				}

				if (i % GroupWidth != 0 && i % GroupWidth != GroupWidth - 1)
				{
					if (i < GroupWidth)
					{
						// �E�ƍ��Ɖ��ɍ쐬
						int leftgate = (mt() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].use = true;
						g_map[leftgate + z][x].passageNum = i;
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
										g_map[bond][path].passageNum = i;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
										g_map[bond][path].passageNum = i;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].use = true;
							g_map[leftgate + z][path].passageNum = i;
						}

						int bottomgate = (mt() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;
						g_map[height][bottomgate + x].passageNum = i + GroupWidth;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
							g_map[path][bottomgate + x].passageNum = i + GroupWidth;
						}

						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;
						g_map[rightgate + z][width].passageNum = i + 1;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
							g_map[rightgate + z][path].passageNum = i + 1;
						}
					}
					else if (i >(GroupHeight - 1) * GroupWidth)
					{
						// �E�ƍ��Ə�ɍ쐬
						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;
						if (GroupHeight <= GroupWidth)
							g_map[rightgate + z][width].passageNum = i + (GroupHeight * GroupWidth / 2 + GroupHeight - 2);
						else
							g_map[rightgate + z][width].passageNum = i + (GroupHeight * GroupWidth / 2 + GroupWidth - 2);
						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
							if (GroupHeight <= GroupWidth)
								g_map[rightgate + z][path].passageNum = i + (GroupHeight * GroupWidth / 2 + GroupHeight - 2);
							else
								g_map[rightgate + z][path].passageNum = i + (GroupHeight * GroupWidth / 2 + GroupWidth - 2);
						}

						int topgate = (mt() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;
						g_map[z][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth + 1;

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
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth + 1;
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth + 1;
									}
								}

							}
							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;
							g_map[path][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + GroupWidth + 1;
						}

						int leftgate = (mt() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].use = true;
						g_map[leftgate + z][x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + 1;

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
										g_map[bond][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + 1;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
										g_map[bond][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + 1;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].use = true;
							g_map[leftgate + z][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + 1;
						}
					}
					else
					{
						// �S�����ɍ쐬
						int rightgate = (mt() % (height - z - 1)) + 1;
						g_map[rightgate + z][width].type = 3;
						g_map[rightgate + z][width].use = true;
						g_map[rightgate + z][width].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + i % GroupWidth + 1;

						// �E�ʘH�̍쐬
						for (int path = width + 1; g_map[rightgate + z][path - 1].Group == i + 1; path++)
						{
							g_map[rightgate + z][path].type = 2;
							g_map[rightgate + z][path].use = true;
							g_map[rightgate + z][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + i % GroupWidth + 1;
						}

						int topgate = (mt() % (width - x - 1)) + 1;
						g_map[z][topgate + x].type = 3;
						g_map[z][topgate + x].use = true;
						g_map[z][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + i % GroupWidth + GroupWidth;
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
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + i % GroupWidth + GroupWidth;
									}
								}
								else
								{
									for (int bond = topgate + x; bond != bottom; bond++)
									{
										g_map[path][bond].type = 2;
										g_map[path][bond].use = true;
										g_map[path][bond].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + i % GroupWidth + GroupWidth;
									}
								}

							}
							g_map[path][topgate + x].type = 2;
							g_map[path][topgate + x].use = true;
							g_map[path][topgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth - 1) + i % GroupWidth + GroupWidth;
						}

						int leftgate = (mt() % (height - z - 1)) + 1;
						g_map[leftgate + z][x].type = 3;
						g_map[leftgate + z][x].use = true;
						g_map[leftgate + z][x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + i % GroupWidth;

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
										g_map[bond][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + i % GroupWidth;
									}
								}
								else
								{
									for (int bond = leftgate + z; bond != right; bond++)
									{
										g_map[bond][path].type = 2;
										g_map[bond][path].use = true;
										g_map[bond][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + i % GroupWidth;
									}
								}

							}
							g_map[leftgate + z][path].type = 2;
							g_map[leftgate + z][path].use = true;
							g_map[leftgate + z][path].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + i % GroupWidth;
						}

						int bottomgate = (mt() % (width - x - 1)) + 1;
						g_map[height][bottomgate + x].type = 3;
						g_map[height][bottomgate + x].use = true;
						g_map[height][bottomgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + i % GroupWidth + GroupWidth;

						// ���ʘH�̍쐬
						for (int path = height + 1; g_map[path - 1][bottomgate + x].Group == i + 1; path++)
						{
							g_map[path][bottomgate + x].type = 2;
							g_map[path][bottomgate + x].use = true;
							g_map[path][bottomgate + x].passageNum = (GroupWidth + GroupWidth - 1) * (i / GroupWidth) + i % GroupWidth + GroupWidth;
						}
					}
				}
			}
		}
	}

	//===================================================
	// �ʘH���폜
	//===================================================
	for (int i = 0; i < DeletePassageNum; i++)
	{
		std::uniform_int_distribution<int> Passagenum(0, (GroupWidth + GroupWidth - 1) * (GroupHeight - 1) + (GroupWidth - 1));
		MapdeletePassage(Passagenum(mt));
		Passagenum(mt);
		Passagenum(mt);
	}


	// ���߂�����ɉ����ĕǂ�z�u
	//===================================================
	// �ǂ𐶐�
	//===================================================
	MapWallSet();
	//===================================================
	// ��q�𐶐�
	//===================================================
	MapLadderSet();
	//===================================================
	// �A�C�e���𐶐�
	//===================================================
	MapItemSet();
	//===================================================
	// �����𐶐�
	//===================================================
	MapWeponSet();
	//===================================================
	// ���i�𐶐�
	//===================================================
	MapTrapSet();
	//===================================================
	// �v���C���[�̏����ʒu�����߂�
	//===================================================
	MapPlayerSet();
	//===================================================
	// �G�𐶐� GroupWidth * GroupHeight - 0�`2�̐���
	//===================================================
	MapEnemySet();
}

void CMap::MapdeletePassage(int passagenum)
{
	for (int z = 0; z < MAX_MAPHEIGHT; z++)
	{

		for (int x = 0; x < MAX_MAPWIDTH; x++)
		{
			if (g_map[z][x].passageNum == passagenum && g_map[z][x].type == 2)
			{
				g_map[z][x].type = 0;
				g_map[z][x].use = false;
			}

			if (g_map[z][x].passageNum == passagenum && g_map[z][x].type == 3)
				g_map[z][x].type = 1;
		}
	}
}

void CMap::MapPlayerPosSet(int mapz, int mapx, int oldz, int oldx)
{
	if (g_map[oldz][oldx].have == HAVEPLAYER)
		g_map[oldz][oldx].have = NOTHAVE;
	if (g_map[mapz][mapx].have == NOTHAVE)
		g_map[mapz][mapx].have = HAVEPLAYER;
}
void CMap::MapEnemyPosSet(int mapz, int mapx, int oldz, int oldx)
{
	if (g_map[oldz][oldx].have == HAVEENEMY)
		g_map[oldz][oldx].have = NOTHAVE;
	if (g_map[mapz][mapx].have == NOTHAVE)
		g_map[mapz][mapx].have = HAVEENEMY;
}