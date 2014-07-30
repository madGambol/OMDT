
#ifndef _baseMap_h
#define _baseMap_h

template <typename T, typename N>
class CBaseMap 
{
    std::multimap< T, N > mTtoN;
    std::multimap< N, T > mNtoT;

    N mDummyN;
    T mDummyT;

public:

    typedef typename std::multimap< T, N >::iterator       TtoNiterator;
    typedef typename std::multimap< T, N >::const_iterator const_TtoNiterator;

    typedef T P1;
    typedef N P2;

    CBaseMap()
    :
    mDummyN( N() ),
    mDummyT( T() )
    {
    }

    ~CBaseMap()
    {
    }

    // 
    // returns true if there are multiple entries
    //
    bool add( const T & item, N & node)
    {
        mTtoN.insert( typename std::multimap< T, N >::value_type( item, node ) );
        mNtoT.insert( typename std::multimap< N, T >::value_type( node, item ) );

        return (mTtoN.count( item ) > 1) || (mNtoT.count( node ) > 1);
    }

    //
    // T is used as the key
    //
    bool range   ( const T & item ) const
    {
        return mTtoN.count( item ) > 1;
    }

    bool contains( const T & item ) const
    {
        return mTtoN.find( item ) != mTtoN.end();
    }

    void getRange( const T & item, 
                   typename std::multimap< T, N >::const_iterator & begin, 
                   typename std::multimap< T, N >::const_iterator & end) const
    {
        begin = mTtoN.lower_bound( item );
        end   = mTtoN.upper_bound( item );
    }

    // get the first 
    const N & getN( const T & item ) const
    {
        typename std::multimap< T, N >::const_iterator itemIter = mTtoN.find( item );

        if (itemIter == mTtoN.end()) return mDummyN;

        return (*itemIter).second;
    }

    // get the first 
    const T & getT( const N & item ) const
    {
        typename std::multimap< T, N >::const_iterator itemIter = mNtoT.find( item );

        if (itemIter == mNtoT.end()) return mDummyT;

        return (*itemIter).second;
    }

    const_TtoNiterator beginN(void) const
    {
        return mTtoN.begin();
    }

    const_TtoNiterator endN  (void) const
    {
        return mTtoN.end();
    }

    //
    // N is used as the key
    //
    bool range   ( const N & item ) const
    {
        return mNtoT.count( item ) > 1;
    }

    bool contains( const N & item ) const
    {
        return mNtoT.find( item ) != mNtoT.end();
    }

    void getRange( N & item, 
                   typename std::multimap< N, T >::const_iterator & begin, 
                   typename std::multimap< N, T >::const_iterator & end)
    {
        begin = mNtoT.lower_bound( item );
        end   = mNtoT.upper_bound( item );
    }

    const_TtoNiterator getLowerN(const T & item) const
    {
        return mTtoN.lower_bound( item );
    }

    const_TtoNiterator getUpperN(const T & item) const
    {
        return mTtoN.upper_bound( item );
    }
  
    // get the first 
    const T & get( const N & item ) const
    {
        typename std::multimap< N, T >::const_iterator itemIter = mNtoT.find( item );

        if (itemIter == mNtoT.end()) return mDummyT;

        return (*itemIter).second;
    }

    typename std::multimap< N, T >::const_iterator beginT() const
    {
        return mNtoT.begin();
    }

    typename std::multimap< N, T >::const_iterator endT  () const
    {
        return mNtoT.end();
    }

    size_t getSizeN(void) const
    {
        return mNtoT.size();
    }

    size_t getSizeT(void) const
    {
        return mTtoN.size();
    }
};

#endif
// Local Variables:
// mode:C++
// End:
