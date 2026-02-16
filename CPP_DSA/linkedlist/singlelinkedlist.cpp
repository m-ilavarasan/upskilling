#include<iostream>

using namespace std;

class Node{
    public:
        int data;
        Node* next;

   Node(int data=0):data(data),next(nullptr)
    {
        //next= NULL;
        //head = NULL;
    }
};
class LinkedList{
    Node* head;
public:
    LinkedList():head(nullptr){}
    Node* createNode(int data)
    {
        Node* head = new Node(data);
        cout << " Creating node \n";
        head->data = data;
        cout <<"1111111111\n";
        head->next = NULL;
        cout <<"Return created node \n";
        return head;
    }
   void insertNode(int data)
    {
        Node* node = new Node(data);
        if(head == NULL)
        {
            head = node;
            head->next = NULL;

        }
        else
        {
            Node* last = head;
            while(last->next!=NULL)
            {
                last = last->next;
            }
           last->next = node;
           
        }
    }

    void insertAtBeginning(int data)
    {
        Node* node = new Node(data);
        cout << "Inserting " << data << " at beginning" << endl;
        
        if(head == NULL)
        {
            head = node;
            head->next = NULL;
        }
        else
        {
            node->next = head;  // New node points to current head
            head = node;         // New node becomes the head
        }
    }

    void deleteNode(int data)
    {
        if(head == NULL) return;
        
        // Delete from head if it matches
        while(head != NULL && head->data == data)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        
        // Delete from rest of the list
        Node* current = head;
        while(current != NULL && current->next != NULL)
        {
            if(current->next->data == data)
            {
                Node* temp = current->next;
                current->next = temp->next;
                delete temp;
            }
            else
            {
                current = current->next;
            }
        }
    }
    void reverseList()
    {
        Node* prev = NULL;
        Node* curr = head;
        Node* nxt = NULL;
        while(curr != NULL)
        {
            nxt = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nxt;
        }
        head = prev;
    }
    void display()
    {
        Node* temp = head;
        cout <<" Data now:" << temp->data <<endl;
        while(temp !=NULL)
        {
            cout <<"data :" << temp->data <<endl;
            temp = temp->next;
        }
    }

    void createCycle(int pos)
    {
        if(head == NULL || pos < 0) return;
        
        Node* current = head;
        Node* cycleNode = NULL;
        int count = 0;
        
        // Find the node at position 'pos' and traverse to the end
        while(current->next != NULL)
        {
            if(count == pos)
            {
                cycleNode = current;
            }
            current = current->next;
            count++;
        }
        
        // Create cycle by connecting last node to node at position 'pos'
        if(cycleNode != NULL)
        {
            current->next = cycleNode;
            cout << "Cycle created at position " << pos << endl;
        }
    }

    bool detectCycle()
    {
        if(head == NULL) return false;
        
        Node* slow = head;
        Node* fast = head;
        
        // Floyd's Cycle Detection Algorithm
        while(fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
            
            if(slow == fast)
            {
                cout << "Cycle detected!" << endl;
                return true;
            }
        }
        cout << "No cycle found!" << endl;
        return false;
    }
};

int main()
{
    LinkedList ll;
    int i = 5;
   
   // Insert nodes at end (5,6,7,8,9)
   for (i; i<10; i++)
   {
    ll.insertNode(i);
   }
   
   cout << "\n--- Original List (inserted at end) ---" << endl;
   ll.display();
   
   cout << "\n--- Inserting at Beginning ---" << endl;
   ll.insertAtBeginning(100);
   ll.insertAtBeginning(50);
   
   cout << "\n--- List After Inserting at Beginning ---" << endl;
   ll.display();
   
   cout << "\n--- Testing Cycle Detection (No Cycle) ---" << endl;
   ll.detectCycle();
   
   cout << "\n--- Creating Cycle at Position 1 ---" << endl;
   ll.createCycle(1);
   
   cout << "\n--- Testing Cycle Detection (With Cycle) ---" << endl;
   ll.detectCycle();
    
    return 0;

}