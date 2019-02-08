
#include <velodyne_pointcloud/organized_cloudXYZIR.h>

namespace velodyne_pointcloud
{

  OrganizedCloudXYZIR::OrganizedCloudXYZIR(
      const double max_range, const double min_range,
      const std::string& target_frame, const std::string& fixed_frame,
      const unsigned int num_lasers, const unsigned int scans_per_block)
    : DataContainerBase(
        max_range, min_range, target_frame, fixed_frame,
        num_lasers, 0, false, scans_per_block){
  }

  void OrganizedCloudXYZIR::newLine()
  {
    if(config_.transform)
    {
      computeTransformation(pcl_conversions::fromPCL(pc->header.stamp));
    }
    for (int j = 0; j < organized_lasers.size(); j++)
    {
      velodyne_rawdata::VPoint *point = organized_lasers[j];
      if(config_.transform)
      {
        transformPoint(*point);
      }
      pc->points.push_back(*point);
      delete point;
    }
    // append this point to the cloud
    ++pc->height;
  }

  void OrganizedCloudXYZIR::addPoint(const float& x, const float& y, const float& z, const uint16_t& ring, const uint16_t& /*azimuth*/, const float& distance, const float& intensity)
  {
    velodyne_rawdata::VPoint *point_ptr = new velodyne_rawdata::VPoint;
    /** The laser values are not ordered, the organized structure
     * needs ordered neighbour points. The right order is defined
     * by the laser_ring value. First collect each laser point
     * and insert them into a map, ordered by the ring number
     */
    organized_lasers[ring] = point_ptr;
    point_ptr->ring = ring;
    if (pointInRange(distance))
    {
      point_ptr->x = x;
      point_ptr->y = y;
      point_ptr->z = z;
      point_ptr->intensity = intensity;
    }
    else
    {
      point_ptr->x = nan("");
      point_ptr->y = nan("");
      point_ptr->z = nan("");
      point_ptr->intensity = 0;
    }
  }
}

