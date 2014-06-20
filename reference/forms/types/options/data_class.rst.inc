data_class
~~~~~~~~~~

**データ型**: ``string``

このオプションは、フォームで使用する適切なデータ·マッパーを設定するために使われます。
オブジェクトを必要とする任意のフィールドタイプに使用することができます。

.. code-block:: php

    $builder->add('media', 'sonata_media_type', array(
        'data_class' => 'Acme\DemoBundle\Entity\Media',
    ));

.. 2014/06/20 yositani2002 69a2ed1f816c209e477ea3560782f5a208c6f0a6
