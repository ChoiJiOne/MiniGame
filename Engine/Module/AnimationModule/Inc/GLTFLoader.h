#pragma once

#include <string>
#include <vector>

#include <cgltf.h>

#include "Mat.h"
#include "Transform.h"
#include "Vec.h"

#include "Clip.h"
#include "Pose.h"
#include "Skeleton.h"


/**
 * @brief GLTF 파일과 파일 내의 데이터 로딩을 수행합니다.
 *
 * @note 이 클래스의 멤버 변수와 메서드는 모두 static 입니다.
 */
class GLTFLoader
{
public:
	/**
	 * @brief 메시 데이터입니다.
	 */
	struct MeshData
	{
		std::vector<Vec3f> positions;
		std::vector<Vec3f> normals;
		std::vector<Vec2f> texcoords;
		std::vector<Vec4f> weights;
		std::vector<Vec4i> joints;
		std::vector<uint32_t> indices;
	};


public:
	/**
	 * @brief GLTF 파일을 로딩합니다.
	 * 
	 * @param path GLTF 파일의 경로입니다.
	 * 
	 * @return GLTF 데이터의 포인터를 반환합니다.
	 * 
	 * @note 이 메서드를 이용해서 얻은 포인터는 반드시 Free 메서드를 이용해서 해제해야 합니다.
	 */
	static cgltf_data* Load(const std::string& path);


	/**
	 * @brief GLTF 데이터 할당을 해제합니다.
	 *
	 * @param data 할당을 해제할 GLTF 데이터입니다.
	 */
	static void Free(cgltf_data* data);


	/**
	 * @brief GLTF 데이터로부터 Rest 포즈를 로딩합니다.
	 * 
	 * @param data GLTF 데이터입니다.
	 * 
	 * @return GLTF로부터 추출된 Rest 포즈를 반환합니다.
	 */
	static Pose LoadRestPose(cgltf_data* data);


	/**
	 * @brief GLTF 데이터로부터 Bind 포즈를 로딩합니다.
	 * 
	 * @param data GLTF 데이터입니다.
	 *
	 * @return GLTF로부터 추출된 Bind 포즈를 반환합니다.
	 */
	static Pose LoadBindPose(cgltf_data* data);


	/**
	 * @brief GLTF 데이터로부터 관절(Joint)의 이름 목록을 로딩합니다.
	 * 
	 * @param data GLTF 데이터입니다.
	 * 
	 * @return GLTF로부터 추출된 관절(Joint)의 이름 목록을 반환합니다.
	 */
	static std::vector<std::string> LoadJointNames(cgltf_data* data);


	/**
	 * @brief GLTF 데이터로부터 스켈레톤 정보를 로딩합니다.
	 * 
	 * @param data GLTF 데이터입니다.
	 * 
	 * @return GLTF로부터 추출된 스켈레톤을 반환합니다.
	 */
	static Skeleton LoadSkeleton(cgltf_data* data);


	/**
	 * @brief GLTF 데이터로부터 애니메이션 클립 목록을 로딩합니다.
	 * 
	 * @param data GLTF 데이터입니다.
	 * 
	 * @return GLTF로부터 추출된 애니메이션 클립 목록을 반환합니다.
	 */
	static std::vector<Clip> LoadAnimationClips(cgltf_data* data);


	/**
	 * @brief GLTF 데이터로부터 메시 데이터를 로딩합니다.
	 * 
	 * @param data GLTF 데이터입니다.
	 * 
	 * @return GLTF로부터 추출된 메시 데이터 목록을 반환합니다.
	 */
	static std::vector<MeshData> LoadMeshData(cgltf_data* data);


private:
	/**
	 * @brief GLTF 노드로부터 로컬 트랜스폼을 얻습니다.
	 * 
	 * @param node 로컬 트랜스폼을 얻을 GLTF 노드입니다.
	 * 
	 * @return GLTF 노드의 로컬 트랜스폼 값을 반환합니다.
	 */
	static Transform GetLocalTransform(cgltf_node* node);


	/**
	 * @brief GLTF 노드의 인덱스를 얻습니다.
	 * 
	 * @param target 인덱스를 얻을 타겟 노드입니다.
	 * @param nodes 노드 목록입니다.
	 * @param numNodes 노드 목록의 수입니다.
	 * 
	 * @return GLTF 노드의 인덱스 값을 반환합니다
	 */
	static int32_t GetNodeIndex(cgltf_node* target, cgltf_node* nodes, uint32_t numModes);


	/**
	 * @brief GLTF의 스칼라 값을 얻습니다.
	 * 
	 * @param outScalars GLTF의 스칼라 값입니다.
	 * @param componentCount 스칼라 값의 컴포넌트 수입니다.
	 * @param accessor 스칼라 데이터의 형식입니다.
	 */
	static void GetScalarValues(std::vector<float>& outScalars, uint32_t componentCount, const cgltf_accessor* accessor);



	/**
	 * @brief GLTF의 채널로부터 트랙 값을 얻습니다.
	 * 
	 * @param inOutTrack GLTF의 채널로부터 트랙 값을 저장할 참조자입니다. 
	 * @param channel GLTF 애니메이션 채널입니다.
	 */
	template <typename T, uint32_t N>
	static void TrackFromChannel(Track<T, N>& inOutTrack, const cgltf_animation_channel* channel);


	/**
	 * @brief GLTF로 부터 메시 데이터를 얻습니다.
	 * 
	 * @param outMesh GLTF 메시 데이터입니다.
	 * @param attribute 정점의 속성입니다.
	 * @param skin 스킨 정보입니다.
	 * @param nodes 노드 목록입니다.
	 * @param numNodes 노드 목록의 수입니다.
	 */
	static void GetMeshFromAttribute(MeshData& outMesh, cgltf_attribute* attribute, cgltf_skin* skin, cgltf_node* nodes, uint32_t numNodes);
};