#ifndef PACKETPP_PAYLOAD_LAYER
#define PACKETPP_PAYLOAD_LAYER

#include "Layer.h"

/// @file

/**
 * \namespace pcpp
 * \brief The main namespace for the PcapPlusPlus lib
 */
namespace pcpp
{

	/**
	 * @class PayloadLayer
	 * Represents a generic or unknown layer or a packet payload
	 */
	class PayloadLayer : public Layer
	{
	public:
		 /** A constructor that creates the layer from an existing packet raw data
		 * @param[in] data A pointer to the raw data
		 * @param[in] dataLen Size of the data in bytes
		 * @param[in] prevLayer A pointer to the previous layer
		 * @param[in] packet A pointer to the Packet instance where layer will be stored in
		 */
		PayloadLayer(uint8_t* data, size_t dataLen, Layer* prevLayer, Packet* packet) : Layer(data, dataLen, prevLayer, packet) { m_Protocol = GenericPayload; }

		/**
		 * A constructor that allocates a new payload
		 * @param[in] data A raw buffer that will be used as a payload. This data will be copied to the layer
		 * @param[in] dataLen The raw buffer length
		 * @param[in] dummy A dummy parameter to separate the constructor signature from the other constructor. Its value isn't used anywhere
		 * @todo dummy is probably not necessary anymore. Remove it
		 */
		PayloadLayer(const uint8_t* data, size_t dataLen, bool dummy);

		~PayloadLayer() {}

		/**
		 * Get a pointer to the payload data
		 * @return A pointer to the payload data
		 */
		uint8_t* getPayload() const { return m_Data; }

		/**
		 * Get the payload data length
		 * @return The payload data length in bytes
		 */
		size_t getPayloadLen() const { return m_DataLen; }

		// implement abstract methods

		/**
		 * Does nothing for this layer (PayloadLayer is always last)
		 */
		void parseNextLayer() {}

		/**
		 * @return Payload data length in bytes
		 */
		size_t getHeaderLen() const { return m_DataLen; }

		/**
		 * Does nothing for this layer
		 */
		void computeCalculateFields() {}

		std::string toString() const;

		OsiModelLayer getOsiModelLayer() const { return OsiModelApplicationLayer; }

	};

} // namespace pcpp

#endif /* PACKETPP_PAYLOAD_LAYER */
