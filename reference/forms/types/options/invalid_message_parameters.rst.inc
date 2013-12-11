invalid_message_parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~

**データ型**: ``array`` **デフォルト**: ``array()``

``invalid_message`` オプションをセットする場合、文字列にいくつかの変数を取り込む必要があるかもしれません。プレースホルダーをそのオプションに加え、変数をこのオプションに含めることで可能です::

    $builder->add('some_field', 'some_type', array(
        // ...
        'invalid_message'            => 'You entered an invalid value - it should include %num% letters',
        'invalid_message_parameters' => array('%num%' => 6),
    ));

.. 2013/12/11 yositani2002 009ec9d4b05789c62ee6549a759471dedb8f557b
