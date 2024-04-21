#include <iostream>
#include <pcl/io/pcd_io.h>//点云文件输入
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/project_inliers.h> //投影滤波
#include <vector>
#include <pcl/kdtree/kdtree_flann.h>  //kdtree近邻搜索
#include <pcl/io/pcd_io.h>  //文件输入输出
#include <pcl/point_types.h>  //点类型相关定义
#include <pcl/visualization/pcl_visualizer.h>//可视化相关定义
#include <pcl/common/distances.h>


using namespace std;


inline void load_pcd(const pcl::PointCloud<pcl::PointXYZ>::Ptr &row)
{
	if(pcl::io::loadPCDFile<pcl::PointXYZ>("/home/xxj/pcl study/light.pcd", *row) == -1){
		PCL_ERROR("无法读取点云文件\n");
		exit(-1);
	}
}


inline void show(const pcl::PointCloud<pcl::PointXYZ>::Ptr &row, const pcl::PointCloud<pcl::PointXYZ>::Ptr &pro, const pcl::PointCloud<pcl::PointXYZ>::Ptr &res)
{
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("cloud show"));
	int v1 = 0;
	int v2 = 1;
	int v3 = 0;
	viewer->createViewPort(0, 0, 0.3, 1, v1);
	viewer->createViewPort(0.3, 0, 0.7, 1, v2);
	viewer->createViewPort(0.7, 0, 1.0, 1 ,v3);
	viewer->setBackgroundColor(0, 0, 0, v1);
	viewer->setBackgroundColor(0.05, 0, 0, v2);
	viewer->setBackgroundColor(0.01, 0.01, 0.01, v3);
    viewer->addText("before", 10, 10, "v1_text", v1);
    viewer->addText("now", 10, 10, "v2_text", v2);
	viewer->addText("new", 10, 10, "v3_text", v3);

	//原始点云绿色
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> src_h(row, 0, 255, 0);
	//投影后的点云
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> pro_cloud(pro, 0, 0, 255);
	//Kd_tree后的点云
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> res_cloud(res, 255, 0, 0);

	//viewer->setBackgroundColor(255, 255, 255);
	viewer->addPointCloud(row, src_h, "cloud", v1);

	viewer->addPointCloud(pro, pro_cloud, "pro_cloud", v2);

	viewer->addPointCloud(res, res_cloud, "res_cloud", v3);


	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
	}
}
	

inline void touying(const pcl::PointCloud<pcl::PointXYZ>::Ptr &row, const pcl::PointCloud<pcl::PointXYZ>::Ptr &pro)
{
	//注释==使用ax+by+cz+d=0的平面模型 创建一个系数为a=c=d=0,b=1的平面,也就是X-Z平面。Y轴相关的点全部投影在X-Z面上
	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
	coefficients->values.resize(4);
	coefficients->values[0] = 0;   //x轴
    coefficients->values[1] = 1.0; //y轴
	coefficients->values[2] = 0;   //z轴
	coefficients->values[3] = 0;   //
	//注释==创建滤波器对象
	pcl::ProjectInliers<pcl::PointXYZ> proj;
	proj.setModelType(pcl::SACMODEL_PLANE);
	proj.setInputCloud(row);
	proj.setModelCoefficients(coefficients);
	proj.filter(*pro);
}

inline void Kd_tree_radius(const pcl::PointCloud<pcl::PointXYZ>::Ptr &pro, const pcl::PointCloud<pcl::PointXYZ>::Ptr &res, const int &K=1990,
                           const float &radius=0.12)
{
   pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
   kdtree.setInputCloud(pro);
   pcl::PointXYZ searchPoint = pro->points[1990];
   std::vector<int> pointIdxKNNSearch(K);
   std::vector<float> pointKNNSquaredDistance(K);

   std::cout<<"K nearest neighbor search at (" << searchPoint.x
            <<" "<< searchPoint.y
			<<" "<< searchPoint.y
            <<") with K=" << K << std::endl;

	if (kdtree.nearestKSearch(searchPoint, K, pointIdxKNNSearch, pointKNNSquaredDistance) > 0) 
	{
        for (size_t i = 0; i < pointIdxKNNSearch.size(); ++i)
		{
            std::cout << "    " << pro->points[pointIdxKNNSearch[i]].x
                         << " " << pro->points[pointIdxKNNSearch[i]].y
                         << " " << pro->points[pointIdxKNNSearch[i]].z
                         << " (squared distance: " << pointKNNSquaredDistance[i] << ")" << std::endl;
						float dis = std::sqrt(pro->points[pointIdxKNNSearch[i]].x * pro->points[pointIdxKNNSearch[i]].x + pro->points[pointIdxKNNSearch[i]].y * pro->points[pointIdxKNNSearch[i]].y + pro->points[pointIdxKNNSearch[i]].z * pro->points[pointIdxKNNSearch[i]].z);
						if (dis > radius)
					        break;
						else
						    res->points.push_back(pro->points[pointIdxKNNSearch[i]]);
		}		
	}	
}

int main(int argc, char** argv)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_projected(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_real(new pcl::PointCloud<pcl::PointXYZ>);

    load_pcd(cloud);
	touying(cloud ,cloud_projected);
	int sum=0;
	for(float i=0.001;i<=0.1;i+=0.001,sum += static_cast<int>(cloud_real->points.size()))
	{
		cloud_real->clear();
	    Kd_tree_radius(cloud_projected,cloud_real,1990,i);	
    }		
		cout<<"sum=   "<< sum << endl;
	show(cloud ,cloud_projected, cloud_real);
	return 0;
}
