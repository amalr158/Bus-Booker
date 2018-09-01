#include <iostream>
#include <fstream>
//#include <conio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
using namespace std;

void clrscr(){}

struct Time {
	int hour, min, sec;
} curr_time;

struct Date {
	int day, mon, year;
} curr_date;

//Function to get corrent date and time
void getCurrentDateAndTime() {
	time_t tt;

	// Declaring variable to store return value of
	// localtime()
	struct tm * ti;

	// Applying time()
	time (&tt);

	// Using localtime()
	ti = localtime(&tt);

	curr_time.hour = ti -> tm_hour;
	curr_time.min = ti -> tm_min;
	curr_time.sec = ti -> tm_sec;

	curr_date.day = ti -> tm_mday;
	curr_date.mon = ti -> tm_mon+1;
	curr_date.year = ti -> tm_year+1900;
}

Time getTime() {
	Time t;
	cout << '\t' << "Hour : ";
	cin >> t.hour;
	cout << '\t' << "Minute : ";
	cin >> t.min;
	cout << '\t' << "Second : ";
	cin >> t.sec;
	return t;
}

Date getDate() {
	Date d ;
	cout << '\t' << "Day : ";
	cin >> d.day;
	cout << '\t' << "Month : ";
	cin >> d.mon;
	cout << '\t' << "Year : ";
	cin >> d.year;
	return d;
}

void showDate(Date d) {
	cout << d.day << '/' << d.mon << '/' << d.year;
}

void showTime(Time t) {
	cout << t.hour << ':' << t.min << ':' << t.sec;
}

/////////////////////////////////////////////////
//Classes
////////////////////////////////////////////////

//Route class Defenition
class Route {
public:
	int route_no;
	char from[30];
	char to[30];
	void getDetails();
	void getRouteDetails();
	void showDetails();
};

//Bus class Defenition
class Bus {
	char bus_name[20];
	int seat[40];
	Time departure;
	Time arrival;
	float price;
public:
	char from[30];
	char to[30];
	int route_no;
	int bus_no;
	Bus();
	void getNewBusNo();
	void getRouteDetails();
	void getDetails();
	void showDetails();
	void showAvailSeats();
	int bookSeats(int, int []);
};

//User class Defenition
class User {
	int seats_booked;
	int seat_nos[6];
	char from[30];
	char to[30];
	char name[30];
	char pwd[20];
	int bus_no;
public:
	char u_name[20];
	void getRouteDetails();
	void getDetails();
	void getSignUpDetails();
	int checkLoginDetails(char *, char *);
	void clientPanel();
	void bookTickets();
	void showTicket();
};

/////////////////////////////////////////////////
//Global Functions
////////////////////////////////////////////////


//Function to add new bus
void addNewBus() {
	fstream file;
	file.open("busses.dat", ios::app | ios::binary);
	Bus b;
	b.getDetails();
	if(file.write((char *)&b, sizeof(b)))
		cout << "New bus has been successfully added." << '\n';
	file.close();
}

//Function to add new route
void addNewRoute() {
	fstream file;
	file.open("routes.dat", ios::app | ios:: binary);
	Route r;
	r.getDetails();
	if(file.write((char *)&r, sizeof(r)))
		cout << "New route has been successfully added." << '\n';
	file.close();
}

//Function to show all bus routes
void showAllRoutes() {
		fstream file;
		Route r;
		file.open("routes.dat", ios::app | ios :: in | ios :: binary);
		cout << "#######################################################################" << '\n';
		cout << "Route no." << "\t\t" << "From" << "\t\t\t" << "To" << '\n';
		cout << "#######################################################################" << '\n' << '\n';
		while(file.read((char *) &r, sizeof(r))) {
			r.showDetails();
			cout << "-----------------------------------------------------------------------" << '\n';
		}
		file.close();
}

//Function to show details of all busses
void showAllBusses() {
		fstream file;
		Bus b;
		file.open("busses.dat", ios::app | ios :: in | ios :: binary);
		cout << "###################################################################################" << '\n';
		cout << "Bus no." << "\t" << "Route no." << "\t" << "Name" << "\t\t" << "Departure" << "\t" << "Arrival" << "\t\t" << "Price" <<'\n';
		cout << "###################################################################################" << '\n' << '\n';
		while(file.read((char *) &b, sizeof(b))) {
			b.showDetails();
			cout << "\n----------------------------------------------------------------------------------" << '\n';
		}
		file.close();
}

void showAvailBusses(char from[30], char to[30]) {
	fstream file;
	Bus b;
	file.open("busses.dat", ios :: app | ios :: in | ios :: binary);
	cout << "###################################################################################" << '\n';
	cout << "Bus no." << "\t" << "Route no." << "\t" << "Name" << "\t\t" << "Departure" << "\t" << "Arrival" << "\t\t" << "Price" <<'\n';
	cout << "###################################################################################" << '\n' << '\n';
	while(file.read((char *) &b, sizeof(b))) {
		if(!strcmp(from, b.from) && !strcmp(to, b.to))
			b.showDetails();
		cout << "\n---------------------------------------------------------------------------------" << '\n';
	}
	file.close();
}

//Function for traveler to signup
void UserSignUp() {
	User u;
	fstream file;
	file.open("Users.dat", ios :: app | ios :: binary);
	u.getSignUpDetails();
	if(file.write((char *) &u, sizeof(u)))
		cout << "Your account has been created successfully." << '\n';
	file.close();
}

//Function for User to login
 void UserLogin() {
	fstream file;
	User u;
	int temp, flag = 0;
	char u_name[20], pwd[20];
	cout << "Enter login details : " << '\n';
	cout << '\t' << "Username : ";
	cin.ignore();
	cin.getline(u_name, 20);
	cout << '\t' << "Password : ";
	cin.getline(pwd, 20);
	file.open("Users.dat", ios :: in | ios :: app | ios :: binary);
	while(file.read((char *) &u, sizeof(u))) {
		temp = u.checkLoginDetails(u_name, pwd);
		if(temp == 1) {
			cout << "You have logged in successfully." << '\n';
			u.clientPanel();
			file.seekg(-1 * sizeof(u), ios :: cur);
			file.write((char *) &u, sizeof(u));
			flag = 1;
			break;
		}
		else if(temp == 3){
			cout << "Incorrect Password." << '\n';
			flag = 1;
			break;
		}
	}
	if(!flag)
		cout << "Username does not exist." << '\n';
	file.close();
}

//Admin Controls
void adminControl() {
	int opt;
	int flag = 0;
	do {
		cout << "Choose a correct option: " << '\n';
		cout << '\t' << "- Add new bus [ 1 ]" << '\n';
		cout << '\t' << "- Add new route [ 2 ]" << '\n';
		cout << '\t' << "- Show all routes [ 3 ]" << '\n';
		cout << '\t' << "- Show all busses [ 4 ]" << '\n';
		cout << '\t' << "- Exit [ 5 ]" << '\n';
		cin>>opt;

		switch(opt) {
			case 1:	addNewBus();
							break;
			case 2:	addNewRoute();
							break;
			case 3:	showAllRoutes();
							break;
			case 4:	showAllBusses();
							break;
			case 5:	flag = 1;
							break;
		}
	}while(!flag);
}

//Client Area
void clientArea() {
	int opt;
	int flag = 0;
	do {
		cout << "Choose a correct option: " << '\n';
		cout << '\t' << "- Login [ 1 ]" << '\n';
		cout << '\t' << "- Create a new account [ 2 ]" << '\n';
		cout << '\t' << "- Exit [ 3 ]" << '\n';
		cin>>opt;

		switch(opt) {
			case 1:	UserLogin();
							break;
			case 2:	UserSignUp();
							break;
			case 3:	flag = 1;
							break;
		}
	}while(!flag);
}


/////////////////////////////////////////////////
//Member functions of class Route
////////////////////////////////////////////////

void Route :: showDetails() {
	cout << route_no << "\t\t\t" << from << "\t\t\t" << to << '\n';
}


void Route :: getDetails() {
	fstream file;
	Route r;
	int flag;
	file.open("routes.dat", ios :: app | ios :: in | ios :: binary);
	int no;
	do {
		no = 100;
		flag = 0;
		cout << "Enter the details of the route : " << '\n';
	 	cout << '\t' << "From : ";
		cin.ignore();
		cin.getline(from, 30);
		cout << '\t' << "To: ";
		cin.getline(to, 30);
		while(file.read((char *) &r, sizeof(r))){
			no+=1;
			if(!strcmp(r.from, from) && !strcmp(r.to, to))
				flag = 1;
		}
	} while(flag);
	route_no = no;
	file.close();
}


/////////////////////////////////////////////////
//Member functions of class bus
////////////////////////////////////////////////

Bus :: Bus() {
	for(int i=0; i<40; i++)
		seat[i] = 0;
}

void Bus :: getNewBusNo() {
	int no = 1000;
	fstream file;
	file.open("busses.dat",ios::app | ios :: binary);
	file.seekg(0, ios :: end);
	no+= (file.tellg() / sizeof(Bus));
	bus_no = no;
	file.close();
}

void Bus :: getRouteDetails() {
	fstream file;
	int flag;
	char temp_from[30];
	char temp_to[30];
	Route r;
	file.open("routes.dat",ios :: app | ios :: in | ios :: binary);
	do {
		flag = 0;
		cout << "Enter the route details : " << '\n';
		cout << '\t' << "From : ";
		cin.sync();
		cin.getline(temp_from, 30);
		cout << '\t' << "To : ";
		cin.getline(temp_to, 30);
		file.seekg(0, ios :: beg);
		while(file.read((char *) &r, sizeof(r))) {
			if(!strcmp(r.from, temp_from) && !strcmp(r.to, temp_to)){
				route_no = r.route_no;
				strcpy(from, r.from);
				strcpy(to, r.to);
				flag = 1;
				break;
			}
		}
	} while(!flag);
	file.close();
}

void Bus :: getDetails() {
	cout << "Enter the details of the bus :" << '\n';
	cout << '\t' << "Name : ";
	cin.ignore();
	cin.getline(bus_name, 20);
	getNewBusNo();
	getRouteDetails();
	cout << "Departure : " << '\n';
	departure = getTime();
	cout << "Arrival : " << '\n';
	arrival = getTime();
	cout << '\t' << "Price : ";
	cin >> price;
}

int Bus :: bookSeats(int n, int sts[6]){
	int flag = 1;
	for(int i=0; i<n; i++){
		if(seat[sts[i]] == 1) {
			cout << "Seat " << sts[i] << "is already booked" << '\n';
			flag = 0;
		}
	}
	if(!flag)
		return 0;
	else {
		for(int i=0; i<n; i++)
			seat[sts[i]] == 1;
		return 1;
	}
}

void Bus :: showDetails() {
	cout << bus_no << "\t" << route_no << "\t\t" << bus_name << "\t\t"; showTime(departure);
	cout << "\t\t";
	showTime(arrival);
	cout << "\t\t" << price;
}

void Bus :: showAvailSeats() {
	int temp=0;
	cout << "\t\t\t" << "  ----------------------------------------------" << '\n';
	for(int i=1; i<=6; i++) {
		cout << "\t\t\t";
		if(i==2 || i==5)
			cout << '(';
		else
			cout << ' ';
		cout << '|' << '\t';
		if(i!=3)
			for(int j=1; j<=8; j++) {
				temp ++;
				cout << "  |";
				if(temp<10)
					cout << 0;
				(seat[temp-1]==0)? cout << temp : cout << "❌";
			}
		else
			cout << '\n';
		cout << '|';
		if(i==2 || i==5)
			cout << ']';
		cout << '\n';
	}
	cout << "\t\t\t" << "  ----------------------------------------------";
}


/////////////////////////////////////////////////
//Member functions of class User
////////////////////////////////////////////////

void User :: getSignUpDetails() {
	int flag;
	fstream file;
	User u;
	file.open("Users.dat", ios :: in | ios :: app | ios :: binary);
	cout << "Enter your details : " << '\n';
	cout << '\t' << "Name : ";
	cin.ignore();
	cin.getline(name, 30);
	do {
		flag = 0;
		cout << '\t' << "Username : ";
		cin.getline(u_name, 20);
		file.seekg(0, ios :: beg);
		while (file.read((char *) &u, sizeof(u))) {
			if(!strcmp(u_name, u.u_name)) {
					flag = 1;
					cout << "Username already exist";
					break;
			}
		}
	} while(flag);
	if(flag)
		cout << '\n';
	cout << '\t' << "Password : ";
	cin.getline(pwd, 20);
	file.close();
}

int User :: checkLoginDetails(char uname[20], char pass[20]) {
	if(!strcmp(uname, u_name) && !strcmp(pass, pwd))
		return 1;
	else if(!strcmp(uname, u_name))
		return 3;
	else if(strcmp)
		return 0;
}

void User :: showTicket() {

}

void User :: bookTickets() {
	char t_from[30], t_to[30];
	Bus b;
	int bus_no, flag;
	Date booking_date;
	cout << "Enter the details : " << '\n';
	cout << '\t' << "From : ";
	cin.ignore();
	cin.getline(t_from, 30);
	cout << '\t' << "To : ";
	cin.getline(t_to, 30);
	cout << '\t' << "Departing(From ";
	showDate(curr_date);
	cout << " to the next 31 days";
	cout << ") : " << '\n';
	booking_date = getDate();
	showAvailBusses(t_from, t_to);
	fstream file;
	file.open("busses.dat", ios :: in| ios:: app | ios :: binary);
	do {
		flag = 1;
		cout << "Choose a valid Bus no (0 - exit) : ";
		cin >> bus_no;
		if(bus_no == 0)
			break;
		file.seekg(0, ios :: beg);
		while (file.read((char *) &b, sizeof(b))) {
			if(!strcmp(t_from, b.from) && !strcmp(t_to, b.to) && bus_no == b.bus_no) {
					int no_of_seats, t_seats[6], conf_booking;
					b.showAvailSeats();
					cout << '\n';
					cout << "Enter the no of seats to buy : ";
					cin >> no_of_seats;
					cout << "Enter seat no and press Enter..." << '\n';
					for(int i=0; i<no_of_seats; i++)
						cin >> t_seats[i];
					conf_booking = b.bookSeats(no_of_seats, t_seats);
					if(conf_booking){
						cout << "Booking successful" << '\n';
						file.seekp(-1 * sizeof(b), ios :: cur);
						file.write((char *) &b, sizeof(b));
						seats_booked = no_of_seats;
						strcpy(from, t_from);
						strcpy(to, t_to);
						for(int i=0; i<no_of_seats; i++)
							seat_nos[i] = t_seats[i];
					}
					flag = 0;
					break;
			}
			cout << "Invalid bus no." << '\n';
		}
	} while(flag);
}

//Client Panel
void User :: clientPanel() {
	int opt;
	int flag = 0;
	do {
		cout << "Choose a correct option: " << '\n';
		cout << '\t' << "- Book Tickets [ 1 ]" << '\n';
		cout << '\t' << "- Show my tickets [ 2 ]" << '\n';
		cout << '\t' << "- Exit [ 3 ]" << '\n';
		cin>>opt;

		switch(opt) {
			case 1:	bookTickets();
							break;
			case 2:	showTicket();
							break;
			case 3:	flag = 1;
							break;
		}
	}while(!flag);
}



int main() {
	getCurrentDateAndTime();
	int opt;
	int flag = 0;
	do {
		cout << '\t' << "- Admin Control [ 1 ]" << '\n';
		cout << '\t' << "- Client Area [ 2 ]" << '\n';
		cout << '\t' << "- Exit [ 3 ]" << '\n';
		cin>>opt;

		switch(opt) {
			case 1:	adminControl();
							break;
			case 2:	clientArea();
							break;
			case 3:	flag = 1;
							break;
		}
	}while(!flag);
	return 0;
}
