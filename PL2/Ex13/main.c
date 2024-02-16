#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
	int i, numPieces = 1000;
	int piecesReceived, notification;
	int check[4][2], moving[4][2];
	int piecesToBePassed[4] = {5, 5, 10, 100};
	int sent = 1, finished = 0;

	for (i = 0; i < 4; i++)
	{if (pipe(check[i]) == -1 || pipe(moving[i]) == -1){
			perror("erro ao criar pipe");
			exit(1);
		}
	}

	printf("Comecou a producao\n");
	for (i = 0; i < 4; i++)
	{

		if (fork() == 0)
		{

			close(moving[i][0]);
			close(check[i][0]);
			if (i == 0)
			{ // Machine 1 has a different process than the other so it has to be
				while (numPieces > 0)
				{
					close(moving[i][0]);
					close(check[i][0]); // worked separately from the others.
					write(moving[i][1], &piecesToBePassed[i], sizeof(piecesToBePassed[i]));
					numPieces -= 5;
					write(check[i][1], &sent, sizeof(sent));

					printf("5 pecas foram enviadas de m1.\n");
				}
			}
			else
			{
				close(moving[i][0]);
				close(check[i][0]);
				int piecesToBeSent = 0;

				read(check[i - 1][1], &notification, sizeof(notification));
				while (notification != 0)
				{
					if (notification == 1)
					{
						read(moving[i - 1][0], &piecesReceived, sizeof(int));
						piecesToBeSent += piecesReceived;

						if (piecesToBeSent == piecesToBePassed[i])
						{
							write(moving[i][1], &piecesToBePassed[i], sizeof(piecesToBePassed[i]));
							write(check[i][1], &sent, sizeof(sent));
							piecesToBeSent = 0;

							if (i == 4){
								printf("%d pieces were sent from  M%d to  A1 and inventory! \n", piecesToBePassed[i], i + 1);
							}
							else
							{
								printf("%d pecas foram envidas de m%d para m%d! \n", piecesToBePassed[i], i + 1, i + 2);
							}
						}
					}
					read(check[i - 1][0], &notification, sizeof(notification));
				}
			}
			write(check[i][1], &finished, sizeof(finished));
			close(moving[i][1]);
			close(check[i][1]);
			exit(EXIT_SUCCESS);
		}
	}

	int storageA = 0, inventory = 0;
	read(check[3][0], &notification, sizeof(notification));

	while (notification != 0)
	{
		if (notification == 1)
		{
			read(moving[3][0], &piecesReceived, sizeof(piecesReceived));
			if (piecesReceived == piecesToBePassed[3])
			{
				storageA += piecesReceived;
				inventory += piecesReceived;
			}
		}
		read(check[3][0], &notification, sizeof(notification));
	}
	close(check[3][1]);
	close(moving[3][1]);
	printf("Peças no armazem A %d \n", storageA);
	printf("Peças no inventario %d \n", inventory);
	return 0;
}