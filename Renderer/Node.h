#pragma once

#include <vector>
#include <memory>
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "RenderObject.h"

namespace TakeTwo
{
    class Node
    {
    public:
        Node(RenderObject* pRenderObject = nullptr);
        Node(const Node&) = delete;
        Node& operator= (const Node&) = delete;
        ~Node();

		inline bool HasRenderObject() { return  mRenderObject != nullptr; }

        void SetRenderObject(RenderObject* pRenderObject);
		RenderObject& GetRenderObject();

        void ApplyTransformation(const std::string& pUniformName);
		void SetName(const std::string& pName);
		std::string GetName() const;

		void SetParent(Node* const pParent);
		const Node* GetParent() const;
		void AddChild(Node* const pChild);
		void RemoveChild(Node* const);
		const std::vector<Node*>& GetChildren() const;

		glm::mat4 GetTransformMatrix();
		glm::mat4 GetLocalTransformMatrix() const;

		void SetPosition(const glm::vec3& pPosition);
		void SetScale(const glm::vec3& pScale);
		void SetRotation(const glm::quat& pRotation);

		glm::vec3 GetPosition() const;
		glm::vec3 GetScale() const;
		glm::quat GetRotation() const;

		Node* GetNextInHierarchy();

	protected:
		std::string mName;

	private:
		void UpdateIfDirty();
		void SetDirty();

		glm::mat4 mWorldTransform;

		glm::vec3 mPosition;
		glm::vec3 mScale;
		glm::quat mRotation;

		bool mDirty;

		Node* mParent;
		Node* mNext;

		std::vector<Node*> mChildren;

		RenderObject* mRenderObject;

		static unsigned int sIndex;

    };
}
