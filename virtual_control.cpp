#include <include/tobii/tobii.h>
#include <include/tobii/tobii_streams.h>

#include "virtual_control.h"

#pragma warning( push )
#pragma warning( disable: 4548 4265 4255 4668 4355 4625 4571 4626 5026 5027 ) // disable warnings triggered by STL code
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#pragma warning( pop )

#define WINVER 0x0500
#include <windows.h>
#include <conio.h>
std::thread* thread1;
float margin_x=0.05;
float margin_y=0.17;
extern bool write_on;
extern float y_centre;
extern float x_centre;
int state[4]={0,0,0,0};
bool state_p[4]={0,0,0,0};
bool pressed[4]={0,0,0,0};
float sens_div=0.02;
//int gl_cnt=0;
int quantiz=16;

int thresh(int x)
{
    if(x<quantiz)
        return x;
    else
        return quantiz;
}
void key_map(INPUT& ip, int b)
{
    switch(b)
    {
    case 0:
        //AFTER VK_HELP
        ip.ki.wVk = 0x41;break;
    case 1:
        ip.ki.wVk = 0x44;//d
        break;
    case 2:
        ip.ki.wVk = 0x57;
        break;
    case 3:
        ip.ki.wVk = 0x53;
    }
}

void rule(float x, float y, INPUT& ip, int b)
{
    key_map(ip,b);
    switch(b)
    {
    case 0:
        //LEFT
        if(x<x_centre-margin_x/2)
        {
            pressed[b]=1;
            state[b]=thresh((0.5-margin_x/2-x)/sens_div);
        }
        else
        {
            pressed[b]=0;
            state[b]=0;
        }
        break;
    case 1:
        //RIGHT
        if(x>x_centre+margin_x/2)
        {
            pressed[b]=1;
            //            std::cout<<(state[b]=thresh(-(0.5+margin_x/2-x)/sens_div))<<std::endl;
            state[b]=thresh(-(0.5+margin_x/2-x)/sens_div);
            //            state[b]=1;

        }
        else
        {
            pressed[b]=0;
            state[b]=0;
        }
        break;
    case 2:
        //FORWARD
        if(y<y_centre)
        {
            pressed[b]=1;
            state[b]=thresh((y_centre-y)/(sens_div/2));
        }
        else
        {
            pressed[b]=0;
            state[b]=0;
        }
        break;
    case 3:
        //BACK
        if(y>y_centre+margin_y/2)
        {

            pressed[b]=1;
            state[b]=quantiz;
        }
        else
        {
            pressed[b]=0;
            state[b]=0;
        }
    }
}


void control(float x, float y, INPUT& ip)
{

    for(int b=0;b<4;b++)
    {
        rule(x,y,ip,b);
    }

    if(write_on)
        for(int gl_cnt=0;gl_cnt<quantiz;gl_cnt++)
        {
            for(int b=0;b<4;b++)
            {
                key_map(ip,b);


                if((gl_cnt==0)&&(pressed[b]))
                {
                    ip.ki.dwFlags = 0; // KEYEVENTF_KEYDOWN for key release
                    SendInput(1, &ip, sizeof(INPUT));
                }
            }

            for(int b=0;b<4;b++)
            {
                key_map(ip,b);
                if(((gl_cnt)>state[b]))
                {
                    ip.ki.dwFlags =KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
                    SendInput(1, &ip, sizeof(INPUT));
                }
                if(!pressed[b])
                {
                    ip.ki.dwFlags =KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
                    SendInput(1, &ip, sizeof(INPUT));
                }
            }
            Sleep(4);
        }


}

static auto list_devices( tobii_api_t* api )
{
    std::vector<std::string> result;
    auto error = tobii_enumerate_local_device_urls( api,
                                                    []( char const* url, void* user_data ) // Use a lambda for url receiver function
    {
        // Add url string to the supplied result vector
        auto list = (std::vector<std::string>*) user_data;
        list->push_back( url );
    }, &result );
    if( error != TOBII_ERROR_NO_ERROR ) std::cerr << "Failed to enumerate devices." << std::endl;

    return result;
}


static auto select_device( std::vector<std::string> const& devices )
{
    auto selection = 0u;
    // Present the available devices and loop until user has selected a valid device
    while( selection < 1 || selection > devices.size() )
    {
        std::cout << std::endl << "Select a device" << std::endl << std::endl;
        auto index = 0;
        for( auto t : devices ) std::cout << ++index << ". " << t << std::endl;

        if( !( std::cin >> selection ) )
        {
            // Clean stdin before reading new value
            std::cin.clear();
            std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
        }
    }

    return devices[ selection - 1 ];
}


static void log( void* log_context, tobii_log_level_t level, char const* text )
{
    // Synchronize access to the log printout
    std::lock_guard<std::mutex> lock(*(std::mutex*)log_context);

    if( level == TOBII_LOG_LEVEL_ERROR )
        std::cerr << "Logged error: " << text << std::endl;
}


static auto reconnect( tobii_device_t* device )
{
    // Try reconnecting for 10 seconds before giving up
    for( int i = 0; i < 40; ++i )
    {
        auto error = tobii_device_reconnect( device );
        if( error != TOBII_ERROR_CONNECTION_FAILED ) return error;
        std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );
    }

    return TOBII_ERROR_CONNECTION_FAILED;
}

void core_close()
{
    thread1->~thread();
}

void core_func()
{
    float *x1, *x2;
    x1=new float;
    x2=new float;

    // Create log mutex used for thread synchronization in log function
    std::mutex log_mutex;
    tobii_custom_log_t custom_log { &log_mutex, log };

    tobii_api_t* api;
    auto error = tobii_api_create( &api, nullptr, &custom_log );
    if( error != TOBII_ERROR_NO_ERROR )
    {
        std::cerr << "Failed to initialize the Tobii Stream Engine API." << std::endl;
        //        return 1;
    }

    auto devices = list_devices( api );
    if( devices.size() == 0 )
    {
        std::cerr << "No stream engine compatible device(s) found." << std::endl;
        tobii_api_destroy( api );
        //        return 1;
    }
    auto selected_device = devices.size() == 1 ? devices[ 0 ] : select_device( devices );
    std::cout << "Connecting to " << selected_device << "." << std::endl;

    tobii_device_t* device;
    error = tobii_device_create( api, selected_device.c_str(), &device );
    if( error != TOBII_ERROR_NO_ERROR )
    {
        std::cerr << "Failed to initialize the device with url " << selected_device << "." << std::endl;
        tobii_api_destroy( api );
        //        return 1;
    }
    // Create atomic used for inter thread communication
    std::atomic<bool> exit_thread( false );
    // Start the background processing thread before subscribing to data
    thread1=new std::thread(
                [x1]()
    {
        INPUT ip;
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0; // hardware scan code for key
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
        Sleep(4000);
        while(1)
        {

            control(*x1, *(x1+1),ip);
        }});

    std::thread thread(
                [&exit_thread, device]()
    {

        while( !exit_thread )
        {
            // Do a timed blocking wait for new gaze data, will time out after some hundred milliseconds
            auto error = tobii_wait_for_callbacks( NULL, 1, &device );

            if( error == TOBII_ERROR_TIMED_OUT ) continue; // If timed out, redo the wait for callbacks call

            if( error == TOBII_ERROR_CONNECTION_FAILED )
            {
                // Block here while attempting reconnect, if it fails, exit the thread
                error = reconnect( device );
                if( error != TOBII_ERROR_NO_ERROR )
                {
                    std::cerr << "Connection was lost and reconnection failed." << std::endl;
                    //                    return;
                }
                continue;
            }
            else if( error != TOBII_ERROR_NO_ERROR )
            {
                std::cerr << "tobii_wait_for_callbacks failed: " << tobii_error_message( error ) << "." << std::endl;
                //                return;
            }
            // Calling this function will execute the subscription callback functions
            error = tobii_device_process_callbacks( device );

            if( error == TOBII_ERROR_CONNECTION_FAILED )
            {
                // Block here while attempting reconnect, if it fails, exit the thread
                error = reconnect( device );
                if( error != TOBII_ERROR_NO_ERROR )
                {
                    std::cerr << "Connection was lost and reconnection failed." << std::endl;
                    //                    return;
                }
                continue;
            }
            else if( error != TOBII_ERROR_NO_ERROR )
            {
                std::cerr << "tobii_device_process_callbacks failed: " << tobii_error_message( error ) << "." << std::endl;
                //                return;
            }
        }
    } );

    // Start subscribing to gaze and supply lambda callback function to handle the gaze point data
    error = tobii_gaze_point_subscribe( device,
                                        []( tobii_gaze_point_t const* gaze_point, void* x1 )
    {
            float y1,y2;
            float *f1;
            float ff=3;
            f1=&ff;
            if( gaze_point->validity == TOBII_VALIDITY_VALID )
    {
            y1=gaze_point->position_xy[ 0 ];
            y2=gaze_point->position_xy[ 1 ];
            //            std::cout << "Gaze point: " << gaze_point->timestamp_us << " " << y1
            //                      << ", " << y2 << std::endl;
            f1=(float*)x1;
            *f1=y1;
            *(f1+1)=y2;
            //            x1=f1;
}
            else
            std::cout << "Gaze point: " << gaze_point->timestamp_us << " INVALID" << std::endl;
}, x1 );
    if( error != TOBII_ERROR_NO_ERROR )
    {
        std::cerr << "Failed to subscribe to gaze stream." << std::endl;
        exit_thread = true;
        thread.join();
        tobii_device_destroy( device );
        tobii_api_destroy( api );
        //        return 1;
    }

    // Main loop
    while( !_kbhit() )
    {
        // Normally, main thread would do things here - simulate this with a sleep
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }

    // Cleanup subscriptions and resources
    error = tobii_gaze_point_unsubscribe( device );
    if( error != TOBII_ERROR_NO_ERROR )
        std::cerr << "Failed to unsubscribe from gaze stream." << std::endl;

    exit_thread = true;
    thread.join();

    error = tobii_device_destroy( device );
    if( error != TOBII_ERROR_NO_ERROR )
        std::cerr << "Failed to destroy device." << std::endl;

    error = tobii_api_destroy( api );
    if( error != TOBII_ERROR_NO_ERROR )
        std::cerr << "Failed to destroy API." << std::endl;

    //    return 0;
}
