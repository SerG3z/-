/*
 * Потокобезопасное бинарное дерево поиска на основе блокировок.
 * Реализовать  (в  виде  класса  или  в  виде  набора  функций)  с  использованием  блокировок 
 * потокобезопасное  бинарное  дерево  поиска.  Потокобезопасное  бинарное  дерево  поиска 
 * должно поддерживать операции добавления, удаления и поиска элементов. 
 * Необходимо разработать тестовую программу, в которой создаётся 
 * p тестовых параллельных потоков, и каждый  поток  выполняет n операций
 * (вид  операций  выбирается  случайным  образом)  с деревом поиска для случайно сгенерированных элементов. 
 * Необходимо оценить пропускную спос обность бинарного дерева поиска
*/

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>

#include <random>
#include <chrono> 

#include <sys/time.h>

int counter_add = 0;
int counter_rem = 0;
int counter_loo = 0;

#define NUM_THREADS 3
#define NUM_OPERATIONS 10


using namespace std;

class TreeNode
{
	public:
		int key;
		string value;
		class TreeNode* left;
		class TreeNode* right;

		TreeNode(int key_, string value_) : key(key_), value(value_), left(NULL), right(NULL) {}
		~TreeNode() {}
};

class Tree
{
	private:
		class TreeNode* root;
		std::mutex mutex;

	public:
		Tree(): root(NULL) {}
		~Tree() {}

		void TreeLock() { mutex.lock(); }
		void TreeUnlock() { mutex.unlock(); }

		void TreeAdd(int key, string value)
		{
			class TreeNode *parent, *newNode, *tree;

			//TreeLock();

			counter_add++;

			tree = root;

			if (!tree) {
				newNode = new class TreeNode(key, value);
				root = newNode;
				//TreeUnlock();
				return;
			}

			for (parent = tree; tree != NULL; ) {
				parent = tree;
				if (key < tree->key) {
					tree = tree->left;
				} else if (key > tree->key) {
					tree = tree->right;
				} else {
					//TreeUnlock(); 
					return;
				}
			}
			
			newNode = new class TreeNode(key, value);
			if (key < parent->key)
				parent->left = newNode;
			else
				parent->right = newNode;
			
			//TreeUnlock();
		}

		void TreeRemove(char dir, class TreeNode *parent, class TreeNode *leaf) 
		{
		    // case 1
		    if ((leaf->left == NULL) && (leaf->right == NULL)) {
		        delete leaf;
		        if (dir == 'l') {
		            parent->left = NULL;
		        } else {
		            parent->right = NULL;
		        }
		        return;
		    // case 2
		    } else if ((leaf->left == NULL) || (leaf->right == NULL)) {
		        if (leaf->left == NULL) {
		            if (dir == 'l') {
		                parent->left = leaf->right;
		            } else {
		                parent->right = leaf->right;
		            }
		        } else if (leaf->right == NULL) {
		           if (dir == 'l') {
		                parent->left = leaf->left;
		            } else {
		                parent->right = leaf->left;
		            } 
		        }
		        delete leaf;
		        return;
		    // case 3
		    } else if ((leaf->left != NULL) && (leaf->right != NULL)) {
		        class TreeNode *min = leaf->right;
		        class TreeNode *pmin = leaf->right;

		        while (min->left != NULL) {
		            pmin = min;
		            min = min->left;
		        }

		        if (dir == 'l') {
		            pmin->left = min->right;
		            parent->left = min;
		            min->left = leaf->left;
		            min->right = leaf->right;
		        } else {
		            pmin->left = min->right;
		            parent->right = min;
		            min->left = leaf->left;
		            min->right = leaf->right;
		        }
		        delete leaf;
		        return;
		    }
		}

		void TreeRemove(int key) 
		{
		    auto *tree = root;
		    auto *parent = root; 
		    char dir;
		    
		    counter_rem++;

		    while (tree != NULL) {
		        if (key == tree->key) {
		            TreeRemove(dir, parent, tree);
		            return;
		        } else if (key < tree->key) {
		            dir = 'l';
		            parent = tree;
		            tree = tree->left;
		        } else if (key > tree->key) {
		            dir = 'r';
		            parent = tree;
		            tree = tree->right;
		        }
		    }
		    return;
		}

		void TreeLookup(int key)
		{
		    class TreeNode *tree = root;

		    counter_loo++;

		    while (tree != NULL) {
		        if (key == tree->key) {
		            return;
		        } else if (key < tree->key) {
		            tree = tree->left;
		        } else if (key > tree->key) {
		            tree = tree->right;
		        }
		    }
		    return;
		}

		void printTree(class TreeNode *leaf) 
		{
		    if (leaf) {
				printTree(leaf->left);
		    	printTree(leaf->right);
		    	cout << leaf->key << " | " << leaf->value << endl;
			}
		}

		void printTree()
		{
			class TreeNode *print_node = root;

		    if (print_node) {
				printTree(print_node->left);
		    	printTree(print_node->right);
		    	cout << "root: " << root->key << " | " << root->value << endl;
			}
		}
};

void test(class Tree *tree)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	// setup psseudo-randomic generator
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(0, 99);

    int branch = distribution(generator);

    if ((branch >= 0) && (branch < 30)) {
    	for (auto i = 0; i < NUM_OPERATIONS; ++i) {
    		tree->TreeLock();
    		tree->TreeAdd(distribution(generator), "node");
    		tree->TreeUnlock();
    	}
    } else if ((branch >= 30) && (branch < 60)) {
    	for (auto i = 0; i < NUM_OPERATIONS; ++i) {
    		tree->TreeLock();
    		tree->TreeRemove(distribution(generator));
    		tree->TreeUnlock();
    	}
    } else {
    	for (auto i = 0; i < NUM_OPERATIONS; ++i) {
    		tree->TreeLock();
    		tree->TreeLookup(distribution(generator));
    		tree->TreeUnlock();
    		
    	}
    }
    return;
}

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}


int main(int argc, char const *argv[])
{
	class Tree bsTree;
	vector<std::thread> th;

	auto t = wtime();

	bsTree.TreeAdd(50, "root");

	for (auto i = 0; i < NUM_THREADS; ++i) {
		th.push_back(std::thread(test, &bsTree));
	}

	for (auto i = 0; i < NUM_THREADS; ++i) {
		th[i].join();
	}

  	t = wtime() - t;

  	cout << counter_add << endl;
  	cout << counter_rem << endl;
  	cout << counter_loo << endl;

  	cout << (double)(NUM_OPERATIONS*NUM_THREADS) / (double)t << endl;

  	bsTree.printTree();

	return 0;
}