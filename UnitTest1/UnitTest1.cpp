#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab12.11(2)/lab12.11(2).cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
        TEST_METHOD(TestInsertOrUpdate_NewNode)
        {
            TreeNode* root = nullptr;
            insertOrUpdate(root, "apple", "яблуко", 1);
            Assert::IsNotNull(root);
            Assert::AreEqual(string("apple"), root->englishWord);
            Assert::AreEqual(string("яблуко"), root->ukrainianWord);
            Assert::AreEqual(1, root->accessCount);
            deleteTree(root);
        }

        TEST_METHOD(TestInsertOrUpdate_UpdateNode)
        {
            TreeNode* root = nullptr;
            insertOrUpdate(root, "apple", "яблуко", 1);
            insertOrUpdate(root, "apple", "яблуко", 1);
            Assert::AreEqual(2, root->accessCount);
            deleteTree(root);
        }

        TEST_METHOD(TestRemove_NodeExists)
        {
            TreeNode* root = nullptr;
            insertOrUpdate(root, "apple", "яблуко", 1);
            insertOrUpdate(root, "banana", "банан", 2);
            bool result = remove(root, "apple");
            Assert::IsTrue(result);
            Assert::IsNull(root->left);
            Assert::AreEqual(string("banana"), root->englishWord);
            deleteTree(root);
        }

        TEST_METHOD(TestRemove_NodeDoesNotExist)
        {
            TreeNode* root = nullptr;
            insertOrUpdate(root, "apple", "яблуко", 1);
            bool result = remove(root, "banana");
            Assert::IsFalse(result);
            deleteTree(root);
        }

        TEST_METHOD(TestFindMaxAccess)
        {
            TreeNode* root = nullptr;
            TreeNode* parent = nullptr;
            insertOrUpdate(root, "apple", "яблуко", 10);
            insertOrUpdate(root, "banana", "банан", 20);
            insertOrUpdate(root, "cherry", "вишня", 5);

            TreeNode* maxNode = findMaxAccess(root, parent);
            Assert::IsNotNull(maxNode);
            Assert::AreEqual(string("banana"), maxNode->englishWord);
            Assert::AreEqual(20, maxNode->accessCount);
            deleteTree(root);
        }
	};
}
