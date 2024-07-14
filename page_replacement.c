#include <stdio.h>
#include <limits.h>

#define MAX_FRAMES 10
#define MAX_PAGES 100

void printFrames(int frames[], int num_frames) {
    printf("Frames: ");
    for (int i = 0; i < num_frames; i++) {
        printf("%d ", frames[i]);
    }
    printf("\n");
}

// FIFO Page Replacement
void fifo(int page_reference[], int num_pages, int num_frames, int *page_faults, int *page_hits) {
    int frames[MAX_FRAMES] = {-1};
    *page_faults = 0;
    *page_hits = 0;
    int index = 0;

    for (int i = 0; i < num_pages; i++) {
        int page = page_reference[i];
        int j, found = 0;

        // Check if page is already in a frame
        for (j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                found = 1;
                (*page_hits)++;
                break;
            }
        }

        if (!found) {
            // Replace page using FIFO
            frames[index] = page;
            index = (index + 1) % num_frames;
            (*page_faults)++;
        }

        printFrames(frames, num_frames);
    }
}

// LRU Page Replacement
void lru(int page_reference[], int num_pages, int num_frames, int *page_faults, int *page_hits) {
    int frames[MAX_FRAMES] = {-1};
    *page_faults = 0;
    *page_hits = 0;
    int frame_index = 0;
    int last_used[MAX_FRAMES] = {-1}; // To track last used time of each frame

    for (int i = 0; i < num_pages; i++) {
        int page = page_reference[i];
        int j, found = 0, lru_index = 0, lru_time = INT_MAX;

        // Check if page is already in a frame
        for (j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                found = 1;
                last_used[j] = i;
                (*page_hits)++;
                break;
            }
        }

        if (!found) {
            // Find the least recently used page
            for (j = 0; j < num_frames; j++) {
                if (last_used[j] < lru_time) {
                    lru_time = last_used[j];
                    lru_index = j;
                }
            }

            // Replace page using LRU
            frames[lru_index] = page;
            last_used[lru_index] = i;
            (*page_faults)++;
        }

        printFrames(frames, num_frames);
    }
}

// Optimal Page Replacement
void optimal(int page_reference[], int num_pages, int num_frames, int *page_faults, int *page_hits) {
    int frames[MAX_FRAMES] = {-1};
    *page_faults = 0;
    *page_hits = 0;

    for (int i = 0; i < num_pages; i++) {
        int page = page_reference[i];
        int j, found = 0;

        // Check if page is already in a frame
        for (j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                found = 1;
                (*page_hits)++;
                break;
            }
        }

        if (!found) {
            // Find the page to replace using optimal
            int replace_index = -1, farthest = i;
            for (j = 0; j < num_frames; j++) {
                int k;
                int next_use = INT_MAX;
                for (k = i + 1; k < num_pages; k++) {
                    if (frames[j] == page_reference[k]) {
                        next_use = k;
                        break;
                    }
                }
                if (next_use == INT_MAX) {
                    replace_index = j;
                    break;
                } else if (next_use > farthest) {
                    farthest = next_use;
                    replace_index = j;
                }
            }

            // Replace page using Optimal
            frames[replace_index] = page;
            (*page_faults)++;
        }

        printFrames(frames, num_frames);
    }
}

int main() {
    int page_reference[MAX_PAGES];
    int num_pages, num_frames;
    int fifo_faults, fifo_hits;
    int lru_faults, lru_hits;
    int optimal_faults, optimal_hits;

    printf("Enter the number of page references: ");
    scanf("%d", &num_pages);

    printf("Enter the page reference string:\n");
    for (int i = 0; i < num_pages; i++) {
        scanf("%d", &page_reference[i]);
    }

    printf("Enter the number of frames: ");
    scanf("%d", &num_frames);

    printf("\nFIFO Page Replacement:\n");
    fifo(page_reference, num_pages, num_frames, &fifo_faults, &fifo_hits);
    printf("Total Page Faults (FIFO): %d\n", fifo_faults);
    printf("Total Page Hits (FIFO): %d\n", fifo_hits);
    printf("Miss Ratio (FIFO): %.2f\n", (float)fifo_faults / num_pages);
    printf("Hit Ratio (FIFO): %.2f\n", (float)fifo_hits / num_pages);

    printf("\nLRU Page Replacement:\n");
    lru(page_reference, num_pages, num_frames, &lru_faults, &lru_hits);
    printf("Total Page Faults (LRU): %d\n", lru_faults);
    printf("Total Page Hits (LRU): %d\n", lru_hits);
    printf("Miss Ratio (LRU): %.2f\n", (float)lru_faults / num_pages);
    printf("Hit Ratio (LRU): %.2f\n", (float)lru_hits / num_pages);

    printf("\nOptimal Page Replacement:\n");
    optimal(page_reference, num_pages, num_frames, &optimal_faults, &optimal_hits);
    printf("Total Page Faults (Optimal): %d\n", optimal_faults);
    printf("Total Page Hits (Optimal): %d\n", optimal_hits);
    printf("Miss Ratio (Optimal): %.2f\n", (float)optimal_faults / num_pages);
    printf("Hit Ratio (Optimal): %.2f\n", (float)optimal_hits / num_pages);

    return 0;
}
