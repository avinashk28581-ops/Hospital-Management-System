#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
using namespace std;
namespace fs=std::filesystem;

#define ID_LOGIN 103
#define ID_PATIENT 201
#define ID_DOCTOR 202
#define ID_DEPARTMENT 203
#define ID_APPOINTMENT 204
#define ID_TREATMENT 205
#define ID_MEDICINE 206
#define ID_PRESCRIPTION 207
#define ID_BILLING 208
#define ID_LOGOUT 209
#define ID_ADD 4001
#define ID_UPDATE 4002
#define ID_DELETE 4003
#define ID_SEARCH 4004
#define ID_VIEW 4005
#define ID_CLEAR 4006
#define ID_BACK 4007
#define ID_LIST 4010
#define ID_FIELD_BASE 3000

HBRUSH blackBrush; HFONT titleFont, headingFont, normalFont, buttonFont;
HWND userBox, passBox; vector<HWND> fields; HWND listBox; int current=-1; HWND moduleWnd=nullptr;

struct Module{wstring title,file;vector<wstring> fields;};
vector<Module> mods={
 {L"PATIENT MANAGEMENT",L"patients.txt",{L"Patient ID",L"Patient Name",L"Age",L"Gender",L"Phone",L"Address"}},
 {L"DOCTOR MANAGEMENT",L"doctors.txt",{L"Doctor ID",L"Doctor Name",L"Specialization",L"Gender",L"Phone",L"Department"}},
 {L"DEPARTMENT MANAGEMENT",L"departments.txt",{L"Department ID",L"Department Name",L"Head Doctor",L"Location"}},
 {L"APPOINTMENT MANAGEMENT",L"appointments.txt",{L"Appointment ID",L"Patient ID",L"Doctor ID",L"Date",L"Time",L"Status"}},
 {L"TREATMENT MANAGEMENT",L"treatments.txt",{L"Treatment ID",L"Patient ID",L"Doctor ID",L"Diagnosis",L"Treatment",L"Date"}},
 {L"MEDICINE MANAGEMENT",L"medicines.txt",{L"Medicine ID",L"Medicine Name",L"Category",L"Quantity",L"Price",L"Expiry Date"}},
 {L"PRESCRIPTION MANAGEMENT",L"prescriptions.txt",{L"Prescription ID",L"Patient ID",L"Doctor ID",L"Medicine",L"Dose",L"Duration"}},
 {L"BILLING MANAGEMENT",L"bills.txt",{L"Bill ID",L"Patient ID",L"Bill Date",L"Description",L"Amount",L"Payment Status"}}
};

void Ensure(){try{fs::create_directories("HospitalData");}catch(...){}}
string N(const wstring&s){string r;for(wchar_t c:s)r+=(c<128?(char)c:'?');return r;}
wstring W(const string&s){wstring r;for(unsigned char c:s)r+=(wchar_t)c;return r;}
wstring Get(HWND h){int n=GetWindowTextLengthW(h);wstring s(n+1,L'\0');if(n)GetWindowTextW(h,s.data(),n+1);s.resize(n);return s;}
string Path(int i){Ensure();return "HospitalData/"+N(mods[i].file);}
vector<string> Split(const string&x){vector<string>v;string a;stringstream ss(x);while(getline(ss,a,'|'))v.push_back(a);return v;}
vector<wstring> Values(){vector<wstring>v;for(HWND h:fields)v.push_back(Get(h));return v;}
void Clear(){for(HWND h:fields)SetWindowTextW(h,L"");}
bool Empty(const vector<wstring>&v){for(auto&s:v)if(s.empty())return true;return false;}
bool Exists(int i,const wstring&id){ifstream f(Path(i));string l;while(getline(f,l)){auto p=Split(l);if(!p.empty()&&W(p[0])==id)return true;}return false;}
bool Add(int i,const vector<wstring>&v){ofstream f(Path(i),ios::app);if(!f)return false;for(size_t k=0;k<v.size();k++){if(k)f<<'|';f<<N(v[k]);}f<<'\n';return true;}
bool Update(int i,const wstring&id,const vector<wstring>&v){ifstream in(Path(i));if(!in)return false;ofstream out("HospitalData/temp.txt");if(!out)return false;bool ok=false;string l;while(getline(in,l)){auto p=Split(l);if(!p.empty()&&W(p[0])==id){for(size_t k=0;k<v.size();k++){if(k)out<<'|';out<<N(v[k]);}out<<'\n';ok=true;}else out<<l<<'\n';}in.close();out.close();remove(Path(i).c_str());rename("HospitalData/temp.txt",Path(i).c_str());return ok;}
bool Del(int i,const wstring&id){ifstream in(Path(i));if(!in)return false;ofstream out("HospitalData/temp.txt");if(!out)return false;bool ok=false;string l;while(getline(in,l)){auto p=Split(l);if(!p.empty()&&W(p[0])==id)ok=true;else out<<l<<'\n';}in.close();out.close();remove(Path(i).c_str());rename("HospitalData/temp.txt",Path(i).c_str());return ok;}
void Load(int i){if(!listBox)return;SendMessageW(listBox,LB_RESETCONTENT,0,0);ifstream f(Path(i));string l;while(getline(f,l))if(!l.empty()){wstring w=W(l);SendMessageW(listBox,LB_ADDSTRING,0,(LPARAM)w.c_str());}}

void DrawButton(LPDRAWITEMSTRUCT d){HBRUSH w=CreateSolidBrush(RGB(255,255,255));FillRect(d->hDC,&d->rcItem,w);DeleteObject(w);FrameRect(d->hDC,&d->rcItem,(HBRUSH)GetStockObject(BLACK_BRUSH));SetBkMode(d->hDC,TRANSPARENT);SetTextColor(d->hDC,RGB(0,0,0));SelectObject(d->hDC,buttonFont);wchar_t t[200]={};GetWindowTextW(d->hwndItem,t,200);DrawTextW(d->hDC,t,-1,&d->rcItem,DT_CENTER|DT_VCENTER|DT_SINGLELINE);}
LRESULT Static(WPARAM w){HDC h=(HDC)w;SetTextColor(h,RGB(255,255,255));SetBkColor(h,RGB(0,0,0));return(LRESULT)blackBrush;}
HWND Label(HWND p,const wstring&t,int x,int y,int width=180,int height=30,HFONT f=nullptr){HWND h=CreateWindowW(L"STATIC",t.c_str(),WS_VISIBLE|WS_CHILD,x,y,width,height,p,nullptr,GetModuleHandleW(nullptr),nullptr);SendMessageW(h,WM_SETFONT,(WPARAM)(f?f:normalFont),TRUE);return h;}
HWND Input(HWND p,int id,int x,int y,int width=340){HWND h=CreateWindowExW(WS_EX_CLIENTEDGE,L"EDIT",L"",WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL,x,y,width,36,p,(HMENU)(INT_PTR)id,GetModuleHandleW(nullptr),nullptr);SendMessageW(h,WM_SETFONT,(WPARAM)normalFont,TRUE);return h;}
HWND Button(HWND p,int id,const wstring&t,int x,int y,int w=180,int h=45){HWND b=CreateWindowW(L"BUTTON",t.c_str(),WS_VISIBLE|WS_CHILD|BS_OWNERDRAW,x,y,w,h,p,(HMENU)(INT_PTR)id,GetModuleHandleW(nullptr),nullptr);SendMessageW(b,WM_SETFONT,(WPARAM)buttonFont,TRUE);return b;}

void Search(int i,const wstring&id){ifstream f(Path(i));string l;while(getline(f,l)){auto p=Split(l);if(!p.empty()&&W(p[0])==id){wstring m=L"Record Found!\n\n";for(size_t k=0;k<p.size()&&k<mods[i].fields.size();k++)m+=mods[i].fields[k]+L": "+W(p[k])+L"\n";MessageBoxW(moduleWnd,m.c_str(),L"Search Result",MB_OK|MB_ICONINFORMATION);return;}}MessageBoxW(moduleWnd,L"Record not found.",L"Search Result",MB_OK|MB_ICONWARNING);}

LRESULT CALLBACK ModuleProc(HWND h,UINT m,WPARAM w,LPARAM l){switch(m){case WM_CTLCOLORSTATIC:return Static(w);case WM_CTLCOLOREDIT:{HDC dc=(HDC)w;SetTextColor(dc,RGB(0,0,0));SetBkColor(dc,RGB(255,255,255));return(LRESULT)GetStockObject(WHITE_BRUSH);}case WM_DRAWITEM:DrawButton((LPDRAWITEMSTRUCT)l);return TRUE;case WM_COMMAND:{switch(LOWORD(w)){case ID_ADD:{auto v=Values();if(Empty(v)){MessageBoxW(h,L"Please enter all fields.",L"Validation",MB_OK|MB_ICONWARNING);break;}if(Exists(current,v[0])){MessageBoxW(h,L"This ID already exists.",L"Duplicate ID",MB_OK|MB_ICONERROR);break;}if(Add(current,v)){MessageBoxW(h,L"Record added successfully.\nData saved permanently.",L"Success",MB_OK|MB_ICONINFORMATION);Load(current);Clear();}break;}case ID_UPDATE:{auto v=Values();if(Empty(v)){MessageBoxW(h,L"Please enter all fields.",L"Validation",MB_OK|MB_ICONWARNING);break;}if(Update(current,v[0],v)){MessageBoxW(h,L"Record updated successfully.",L"Success",MB_OK|MB_ICONINFORMATION);Load(current);}else MessageBoxW(h,L"ID not found.",L"Update",MB_OK|MB_ICONWARNING);break;}case ID_DELETE:{if(fields.empty())break;wstring id=Get(fields[0]);if(id.empty()){MessageBoxW(h,L"Enter ID to delete.",L"Delete",MB_OK|MB_ICONWARNING);break;}if(MessageBoxW(h,L"Are you sure you want to delete this record?",L"Confirm Delete",MB_YESNO|MB_ICONQUESTION)==IDYES){if(Del(current,id)){MessageBoxW(h,L"Record deleted successfully.",L"Success",MB_OK|MB_ICONINFORMATION);Load(current);Clear();}else MessageBoxW(h,L"ID not found.",L"Delete",MB_OK|MB_ICONWARNING);}break;}case ID_SEARCH:{wstring id=fields.empty()?L"":Get(fields[0]);if(id.empty())MessageBoxW(h,L"Enter ID to search.",L"Search",MB_OK|MB_ICONWARNING);else Search(current,id);break;}case ID_VIEW:Load(current);break;case ID_CLEAR:Clear();break;case ID_BACK:DestroyWindow(h);break;}break;}case WM_CLOSE:DestroyWindow(h);break;case WM_DESTROY:moduleWnd=nullptr;listBox=nullptr;fields.clear();current=-1;break;default:return DefWindowProcW(h,m,w,l);}return 0;}

void ShowModule(int i,HWND parent){current=i;fields.clear();const wchar_t C[]=L"HospitalModule";WNDCLASSW wc={};wc.lpfnWndProc=ModuleProc;wc.hInstance=GetModuleHandleW(nullptr);wc.lpszClassName=C;wc.hbrBackground=blackBrush;wc.hCursor=LoadCursorW(nullptr,IDC_ARROW);RegisterClassW(&wc);HWND h=CreateWindowExW(0,C,mods[i].title.c_str(),WS_OVERLAPPEDWINDOW,90,40,1150,760,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);moduleWnd=h;Label(h,L"HOSPITAL MANAGEMENT SYSTEM",160,20,820,45,titleFont);Label(h,mods[i].title,250,70,650,40,headingFont);int y=125;for(size_t k=0;k<mods[i].fields.size();k++){Label(h,mods[i].fields[k]+L":",70,y,160,30);fields.push_back(Input(h,ID_FIELD_BASE+(int)k,235,y-4,350));y+=48;}Button(h,ID_ADD,L"ADD",650,125);Button(h,ID_UPDATE,L"UPDATE",650,180);Button(h,ID_DELETE,L"DELETE",650,235);Button(h,ID_SEARCH,L"SEARCH",650,290);Button(h,ID_VIEW,L"VIEW ALL",650,345);Button(h,ID_CLEAR,L"CLEAR",650,400);Button(h,ID_BACK,L"BACK",650,455);Label(h,L"SAVED RECORDS",70,430,500,35,headingFont);listBox=CreateWindowExW(WS_EX_CLIENTEDGE,L"LISTBOX",L"",WS_VISIBLE|WS_CHILD|WS_VSCROLL|WS_HSCROLL|LBS_NOINTEGRALHEIGHT,70,475,930,190,h,(HMENU)(INT_PTR)ID_LIST,GetModuleHandleW(nullptr),nullptr);SendMessageW(listBox,WM_SETFONT,(WPARAM)normalFont,TRUE);Load(i);ShowWindow(h,SW_SHOW);UpdateWindow(h);}

LRESULT CALLBACK DashProc(HWND h,UINT m,WPARAM w,LPARAM l){switch(m){case WM_CTLCOLORSTATIC:return Static(w);case WM_DRAWITEM:DrawButton((LPDRAWITEMSTRUCT)l);return TRUE;case WM_COMMAND:{int id=LOWORD(w);if(id>=ID_PATIENT&&id<=ID_BILLING){ShowModule(id-ID_PATIENT,h);break;}if(id==ID_LOGOUT)DestroyWindow(h);break;}case WM_CLOSE:DestroyWindow(h);break;default:return DefWindowProcW(h,m,w,l);}return 0;}
void Dashboard(HWND p){const wchar_t C[]=L"HospitalDashboard";WNDCLASSW wc={};wc.lpfnWndProc=DashProc;wc.hInstance=GetModuleHandleW(nullptr);wc.lpszClassName=C;wc.hbrBackground=blackBrush;wc.hCursor=LoadCursorW(nullptr,IDC_ARROW);RegisterClassW(&wc);HWND h=CreateWindowExW(0,C,L"Hospital Management System",WS_OVERLAPPEDWINDOW,150,50,1050,720,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);Label(h,L"HOSPITAL MANAGEMENT SYSTEM",100,25,850,50,titleFont);Label(h,L"Welcome, Admin",300,85,450,35,headingFont);const wchar_t* names[]={L"PATIENTS",L"DOCTORS",L"DEPARTMENTS",L"APPOINTMENTS",L"TREATMENTS",L"MEDICINES",L"PRESCRIPTIONS",L"BILLING"};int ids[]={ID_PATIENT,ID_DOCTOR,ID_DEPARTMENT,ID_APPOINTMENT,ID_TREATMENT,ID_MEDICINE,ID_PRESCRIPTION,ID_BILLING};for(int i=0;i<8;i++){int x=(i%2)?540:90,y=155+(i/2)*85;Button(h,ids[i],names[i],x,y,360,60);}Button(h,ID_LOGOUT,L"LOGOUT",345,510,360,60);ShowWindow(h,SW_SHOW);UpdateWindow(h);}

LRESULT CALLBACK LoginProc(HWND h,UINT m,WPARAM w,LPARAM l){switch(m){case WM_CTLCOLORSTATIC:return Static(w);case WM_CTLCOLOREDIT:{HDC dc=(HDC)w;SetTextColor(dc,RGB(0,0,0));SetBkColor(dc,RGB(255,255,255));return(LRESULT)GetStockObject(WHITE_BRUSH);}case WM_DRAWITEM:DrawButton((LPDRAWITEMSTRUCT)l);return TRUE;case WM_COMMAND:if(LOWORD(w)==ID_LOGIN){if(Get(userBox)==L"admin"&&Get(passBox)==L"admin123")Dashboard(h);else MessageBoxW(h,L"Invalid username or password.\n\nUsername: admin\nPassword: admin123",L"Login Failed",MB_OK|MB_ICONERROR);}break;case WM_CLOSE:DestroyWindow(h);break;case WM_DESTROY:PostQuitMessage(0);break;default:return DefWindowProcW(h,m,w,l);}return 0;}

int WINAPI WinMain(HINSTANCE hi,HINSTANCE,LPSTR,int show){Ensure();blackBrush=CreateSolidBrush(RGB(0,0,0));titleFont=CreateFontW(30,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,0,DEFAULT_PITCH,L"Arial");headingFont=CreateFontW(22,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,0,DEFAULT_PITCH,L"Arial");normalFont=CreateFontW(18,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,0,0,0,DEFAULT_PITCH,L"Arial");buttonFont=CreateFontW(16,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,0,DEFAULT_PITCH,L"Arial");const wchar_t C[]=L"HospitalLogin";WNDCLASSW wc={};wc.lpfnWndProc=LoginProc;wc.hInstance=hi;wc.lpszClassName=C;wc.hbrBackground=blackBrush;wc.hCursor=LoadCursorW(nullptr,IDC_ARROW);RegisterClassW(&wc);HWND h=CreateWindowExW(0,C,L"Hospital Management System - Login",WS_OVERLAPPEDWINDOW,250,100,900,600,nullptr,nullptr,hi,nullptr);Label(h,L"HOSPITAL MANAGEMENT SYSTEM",120,55,660,50,titleFont);Label(h,L"ADMIN LOGIN",300,135,300,35,headingFont);Label(h,L"Username:",210,210,130);userBox=Input(h,1001,350,205,300);Label(h,L"Password:",210,270,130);passBox=Input(h,1002,350,265,300);SendMessageW(passBox,EM_SETPASSWORDCHAR,(WPARAM)L'*',0);Button(h,ID_LOGIN,L"LOGIN",350,340,300,50);ShowWindow(h,show);UpdateWindow(h);MSG msg;while(GetMessageW(&msg,nullptr,0,0)){TranslateMessage(&msg);DispatchMessageW(&msg);}return 0;}
