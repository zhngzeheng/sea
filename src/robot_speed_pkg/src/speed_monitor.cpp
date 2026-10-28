#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/int32.hpp>
#include <vector>

using namespace std;

template<class T>
T absoluteDifference(T a, T b)
{
    return (a > b) ? (a - b) : (b - a);
}

class SpeedMonitor : public rclcpp::Node
{
public:
    SpeedMonitor() : Node("speed_monitor")
    {
        sub_ = this->create_subscription<std_msgs::msg::Int32>(
            "/robot_speed", 10, 
            std::bind(&SpeedMonitor::msg_callback, this, std::placeholders::_1));
        
        last_process_speed_ = -1; // 初始化为 -1 表示还没收到过数据
        RCLCPP_INFO_STREAM(get_logger(), "监测节点已启动");
    }

private:
    void msg_callback(const std_msgs::msg::Int32::SharedPtr msg)
    {
        int curr_speed = msg->data;
        
        RCLCPP_INFO_STREAM(get_logger(), "Receive Speed:" << curr_speed);

        // 如果不是第一次接收数据，才进行对比
        if (last_process_speed_ != -1) 
        {
            int diff = absoluteDifference(curr_speed, last_process_speed_);
            
            RCLCPP_INFO_STREAM(get_logger(), "Processing Speed:" << curr_speed);
            RCLCPP_INFO_STREAM(get_logger(), "Previous Speed:" << last_process_speed_);
            RCLCPP_INFO_STREAM(rclcpp::get_logger("monitor"), "Speed Difference:" << diff);
            
            if (diff >= 3) {
                RCLCPP_INFO_STREAM(rclcpp::get_logger("monitor"), "State:WARNING");
            } else {
                RCLCPP_INFO_STREAM(rclcpp::get_logger("monitor"), "State:NORMAL");
            }

            // 将差值存入历史记录，并保持最多 4 个
            diff_vec_.push_back(diff);
            if (diff_vec_.size() > 4)
            {
                diff_vec_.erase(diff_vec_.begin());
            }

            // 统计告警次数
            int warn_count = 0;
            for(auto v : diff_vec_)
            {
                if(v >= 3) warn_count++;
            }
            
            if (warn_count >= 2)   
            {
                RCLCPP_INFO_STREAM(get_logger(), "System State:UNSTABLE");
            }
            else
            {
                RCLCPP_INFO_STREAM(get_logger(), "System State:STABLE");
            }
        }
        else
        {
            RCLCPP_INFO_STREAM(get_logger(), "Waiting for next commend...");
        }

        // 更新上一次的速度，供下一秒使用
        last_process_speed_ = curr_speed;
    }

    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_;
    vector<int> diff_vec_;
    int last_process_speed_;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = make_shared<SpeedMonitor>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}