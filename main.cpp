#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>
using namespace std;

// Structure for Study Material
struct StudyMaterial {
    string name;
    bool isCompleted;

    StudyMaterial(const string& name) : name(name), isCompleted(false) {}
};

// Structure for Quiz Questions
struct QuizQuestion {
    string question;
    string answer;

    QuizQuestion(const string& question, const string& answer)
        : question(question), answer(answer) {}
};

// Structure for Study Plans
struct StudyPlan {
    string subjectName;
    vector<string> materials;
    int plannedHours;
    int hoursStudied;

    StudyPlan(const string& subject, const vector<string>& mats, int hours)
        : subjectName(subject), materials(mats), plannedHours(hours), hoursStudied(0) {}

    double getProgress() const {
        return plannedHours == 0 ? 0.0 : (static_cast<double>(hoursStudied) / plannedHours) * 100.0;
    }
};

// Class for Linked List Node
class ListNode {
public:
    string data;
    ListNode* next;

    ListNode(const string& data) : data(data), next(nullptr) {}
};

// Class for Linked List
class LinkedList {
private:
    ListNode* head;
    ListNode* tail;

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    void add(const string& notification) {
        ListNode* newNode = new ListNode(notification);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void display() const {
        if (!head) {
            cout << "No notifications available.\n";
            return;
        }
        ListNode* current = head;
        cout << "Notifications:\n";
        while (current) {
            cout << "- " << current->data << "\n";
            current = current->next;
        }
    }

    ~LinkedList() {
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Class for managing a subject
class Subject {
public:
    string name;
    vector<StudyMaterial*> materials;
    vector<QuizQuestion> quizQuestions;

    Subject(const string& name) : name(name) {}

    void addMaterial(const string& materialName) {
        materials.push_back(new StudyMaterial(materialName));
    }

    void markMaterialAsCompleted(const string& materialName) {
        for (auto& material : materials) {
            if (material->name == materialName) {
                material->isCompleted = true;
                break;
            }
        }
    }

    void addQuizQuestion(const string& question, const string& answer) {
        quizQuestions.emplace_back(question, answer);
    }

    ~Subject() {
        for (auto& material : materials) {
            delete material;
        }
    }
};

// Graph class for subject connections
class Graph {
private:
    unordered_map<string, vector<string> > adjacencyList;

public:
    void addEdge(const string& subject, const string& relatedSubject) {
        adjacencyList[subject].push_back(relatedSubject);
    }

    void recommendSubjects(const string& subject) {
        cout << "Recommended subjects based on " << subject << ":\n";
        if (adjacencyList.find(subject) != adjacencyList.end()) {
            for (const auto& recommendation : adjacencyList[subject]) {
                cout << recommendation << "\n";
            }
        } else {
            cout << "No recommendations available for this subject.\n";
        }
    }
};

// Stack implementation
class Stack {
private:
    vector<string> items;

public:
    void push(const string& item) {
        items.push_back(item);
    }

    void pop() {
        if (!items.empty()) {
            items.pop_back();
        } else {
            cout << "Stack is empty!\n";
        }
    }

    string top() {
        return items.empty() ? "Stack is empty!" : items.back();
    }

    bool isEmpty() {
        return items.empty();
    }
};

// Queue implementation
class Queue {
private:
    vector<string> items;

public:
    void enqueue(const string& item) {
        items.push_back(item);
    }

    void dequeue() {
        if (!items.empty()) {
            items.erase(items.begin());
        } else {
            cout << "Queue is empty!\n";
        }
    }

    string front() {
        return items.empty() ? "Queue is empty!" : items.front();
    }

    bool isEmpty() {
        return items.empty();
    }
};

// StudyPlanner class
class StudyPlanner {
private:
    vector<Subject*> subjects;
    vector<StudyPlan*> studyPlans;
    Graph graph;
    Stack actionStack;
    Queue sessionQueue;
    LinkedList notifications; // Vector to store notifications
    int totalPoints;

public:
    StudyPlanner() : totalPoints(0) {
        srand(time(0)); // Seed for random score generation
    }

    // // Method to add notifications
    // void addNotification(const string& notification) {
    //     notifications.push_back(notification);
    // }

    // // Method to show all notifications
    // void showNotifications() {
    //     if (notifications.empty()) {
    //         cout << "No notifications available.\n";
    //     } else {
    //         cout << "Notifications:\n";
    //         for (const auto& notification : notifications) {
    //             cout << "- " << notification << "\n";
    //         }
    //     }
    // }

    void addSubject(const string& subjectName) {
        subjects.push_back(new Subject(subjectName));
        actionStack.push("Added subject: " + subjectName);
    }

    void scheduleStudySession(const string& subjectName, const string& date, const string& time) {
        string session = "Study session for " + subjectName + " on " + date + " at " + time;
        sessionQueue.enqueue(session);
        addNotification("Scheduled: " + session);  // Add notification when a session is scheduled
        cout << "Scheduled: " << session << "\n";
    }

    void notifyUpcomingSessions() {
        if (!sessionQueue.isEmpty()) {
            addNotification("Reminder: Your next study session is: " + sessionQueue.front());
            cout << "Reminder: Your next study session is: " << sessionQueue.front() << endl;
        } else {
            addNotification("No upcoming study sessions.");
            cout << "No upcoming sessions.\n";
        }
    }

    void addStudyMaterial(const string& subjectName, const string& materialName) {
        for (auto& subject : subjects) {
            if (subject->name == subjectName) {
                subject->addMaterial(materialName);
                std::cout << "Added material: " << materialName << " to subject: " << subjectName << "\n";
                actionStack.push("Added material: " + materialName + " to subject: " + subjectName);
                return;
            }
        }
        std::cout << "Subject not found!\n";
    }

    void addGraphEdge(const string& subject, const string& relatedSubject) {
        graph.addEdge(subject, relatedSubject);
    }

    void markMaterialAsCompleted(const string& subjectName, const string& materialName) {
        for (auto& subject : subjects) {
            if (subject->name == subjectName) {
                subject->markMaterialAsCompleted(materialName);
                totalPoints += 10; // Earn points for completing material
                std::cout << "Marked material: " << materialName << " as completed in subject: " << subjectName << "\n";
                actionStack.push("Marked material: " + materialName + " as completed in subject: " + subjectName);
                return;
            }
        }
        std::cout << "Subject or material not found!\n";
    }

    void displaySubjectsAndMaterials() {
        cout << "Subjects and their materials:\n";
        for (auto& subject : subjects) {
            cout << "Subject: " << subject->name << "\n";
            for (auto& material : subject->materials) {
                cout << " - " << material->name << (material->isCompleted ? " (Completed)" : "") << "\n";
            }
            // cout << "Progress: " << fixed << setprecision(2) << subject->getProgress() << "%\n";
        }
        cout << "Total Points: " << totalPoints << "\n";
    }

    void recommendSubjects(const string& subjectName) {
        graph.recommendSubjects(subjectName);
    }

    void addQuizQuestion(const string& subjectName, const string& question, const string& answer) {
        for (auto& subject : subjects) {
            if (subject->name == subjectName) {
                subject->addQuizQuestion(question, answer);
                cout << "Added quiz question to " << subjectName << ".\n";
                return;
            }
        }
        cout << "Subject not found!\n";
    }

    void takeQuiz(const string& subjectName) {
        for (auto& subject : subjects) {
            if (subject->name == subjectName) {
                cout << "Taking quiz for " << subjectName << ":\n";
                int score = 0;
                for (const auto& question : subject->quizQuestions) {
                    string userAnswer;
                    cout << question.question << "\nYour answer: ";
                    getline(cin, userAnswer);
                    if (userAnswer == question.answer) {
                        score++;
                    }
                }
                cout << "Your score: " << score << "/" << subject->quizQuestions.size() << "\n";
                return;
            }
        }
        cout << "Subject not found!\n";
    }

    void createStudyPlan(const string& subjectName, const vector<string>& materials, int hours) {
        studyPlans.push_back(new StudyPlan(subjectName, materials, hours));
        actionStack.push("Created study plan for: " + subjectName);
        cout << "Study plan created for " << subjectName << " with " << hours << " hours planned.\n";
    }

    void updateStudyPlan(const string& subjectName, int hoursStudied) {
        for (auto& plan : studyPlans) {
            if (plan->subjectName == subjectName) {
                plan->hoursStudied += hoursStudied;
                cout << "Updated study plan for " << subjectName << ". Total hours studied: " << plan->hoursStudied << "\n";
                actionStack.push("Updated study plan for: " + subjectName);
                return;
            }
        }
        cout << "Study plan not found for subject: " << subjectName << "\n";
    }

    void displayStudyPlans() {
        cout << "Study Plans:\n";
        for (const auto& plan : studyPlans) {
            cout << "Subject: " << plan->subjectName << ", Planned Hours: " << plan->plannedHours
                      << ", Hours Studied: " << plan->hoursStudied
                      << ", Progress: " << fixed << setprecision(2) << plan->getProgress() << "%\n";
            cout << "Materials: ";
            for (const auto& mat : plan->materials) {
                cout << mat << " ";
            }
            cout << "\n";
        }
    }

    void undoLastAction() {
        if (!actionStack.isEmpty()) {
            cout << "Undoing action: " << actionStack.top() << "\n";
            actionStack.pop();
        } else {
            cout << "No actions to undo!\n";
        }
    }

    void viewScheduledSessions() {
        cout << "Scheduled Study Sessions:\n";
        if (sessionQueue.isEmpty()) {
            cout << "No scheduled sessions.\n";
        } else {
            cout << sessionQueue.front() << "\n"; // Display the next session
        }
    }

    ~StudyPlanner() {
        for (auto& plan : studyPlans) {
            delete plan; // Clean up study plans
        }
        for (auto& subject : subjects) {
            delete subject;
        }
    }

    void showNotifications() const {
        notifications.display();  // Use LinkedList to show notifications
    }

    // Add notification
    void addNotification(const string& notification) {
        notifications.add(notification);  // Add a new notification to the linked list
    }
};
int main() {
    StudyPlanner planner;
    string subject, material, date, time, question, answer;
    int choice;

    do {
        cout << "\n1. Add Subject\n";
        cout << "2. Add Study Material\n";
        cout << "3. Schedule Study Session\n";
        cout << "4. Mark Material as Completed\n";
        cout << "5. Display Subjects and Materials\n";
        cout << "6. Recommend Subjects\n";
        cout << "7. Undo Last Action\n";
        cout << "8. View Scheduled Sessions\n";
        cout << "9. Display Notifications\n"; // Updated to show notifications using LinkedList
        cout << "10. Add Quiz Question\n";
        cout << "11. Take Quiz\n";
        cout << "12. Create Study Plan\n";
        cout << "13. Update Study Plan\n";
        cout << "14. Display Study Plans\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore(); // Clear the input buffer

        switch (choice) {
            case 1: {
                cout << "Enter subject name: ";
                getline(cin, subject);
                planner.addSubject(subject);
                planner.addNotification("Subject added: " + subject); // Add notification
                break;
            }
            case 2: {
                cout << "Enter subject name: ";
                getline(cin, subject);
                cout << "Enter material name: ";
                getline(cin, material);
                planner.addStudyMaterial(subject, material);
                planner.addNotification("Material added: " + material + " to " + subject); // Add notification
                break;
            }
            case 3: {
                cout << "Enter subject name: ";
                getline(cin, subject);
                cout << "Enter date (YYYY-MM-DD): ";
                getline(cin, date);
                cout << "Enter time (HH:MM): ";
                getline(cin, time);
                planner.scheduleStudySession(subject, date, time);
                planner.addNotification("Scheduled: Study session for " + subject + " on " + date + " at " + time);
                break;
            }
            case 4: {
                cout << "Enter subject name: ";
                getline(cin, subject);
                cout << "Enter material name: ";
                getline(cin, material);
                planner.markMaterialAsCompleted(subject, material);
                planner.addNotification("Material completed: " + material + " in " + subject); // Add notification
                break;
            }
            case 5:
                planner.displaySubjectsAndMaterials();
                break;
            case 6: {
                cout << "Enter subject name for recommendations: ";
                getline(cin, subject);
                planner.recommendSubjects(subject);
                break;
            }
            case 7:
                planner.undoLastAction();
                break;
            case 8:
                planner.viewScheduledSessions();
                break;
            case 9:
                planner.showNotifications(); // Show notifications using LinkedList
                break;
            case 10: {
                cout << "Enter subject name: ";
                getline(cin, subject);
                cout << "Enter quiz question: ";
                getline(cin, question);
                cout << "Enter answer: ";
                getline(cin, answer);
                planner.addQuizQuestion(subject, question, answer);
                planner.addNotification("Quiz question added to " + subject); // Add notification
                break;
            }
            case 11: {
                cout << "Enter subject name for quiz: ";
                getline(cin, subject);
                planner.takeQuiz(subject);
                break;
            }
            case 12: {
                vector<string> materials;
                int plannedHours;
                cout << "Enter subject name for study plan: ";
                getline(cin, subject);
                cout << "Enter materials (separated by commas): ";
                string mats;
                getline(cin, mats);
                stringstream ss(mats);
                string mat;
                while (getline(ss, mat, ',')) {
                    materials.push_back(mat);
                }
                cout << "Enter planned hours: ";
                cin >> plannedHours;
                cin.ignore(); // Clear the input buffer
                planner.createStudyPlan(subject, materials, plannedHours);
                break;
            }
            case 13: {
                cout << "Enter subject name to update study plan: ";
                getline(cin, subject);
                int hoursStudied;
                cout << "Enter hours studied: ";
                cin >> hoursStudied;
                cin.ignore(); // Clear the input buffer
                planner.updateStudyPlan(subject, hoursStudied);
                break;
            }
            case 14:
                planner.displayStudyPlans();
                break;
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid option! Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}