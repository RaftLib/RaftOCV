#pragma once

#include <raft>

template <typename T>
class DuplicateKernel : public raft::kernel {
    std::size_t  port_name_index = 0;
public:
    DuplicateKernel( const std::size_t num_ports = 2 )
    {
        input.addPort< T >( "0" );

        using index_type = std::remove_const_t<decltype(num_ports)>;
        for( index_type it( 0 ); it < num_ports; it++ )
        {
            addPort();
        }
    }

    virtual size_t addPort()
    {
        const auto portid( port_name_index++ );
        output.addPort< T >( std::to_string( portid ) );
        return( portid );
    }

    raft::kstatus run() override {
        T val;
        this->input[ "0" ].pop(val);

        for(auto& o : output) {
            o.push(val);
        }

        this->input[ "0" ].recycle(this->input[ "0" ].size());

        return( raft::proceed );
    }

};

template <typename T>
static DuplicateKernel<typename T::output_t> CreateDuplicateKernel(const T& kernel, size_t numPorts = 2) {
    return DuplicateKernel<typename T::output_t>(numPorts);
}