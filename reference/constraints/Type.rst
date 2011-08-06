.. 2011/08/06 yanchi d7adfc5a5fde105ac21dc6be872368fc30185a4a

Type
====

値が特定のデータ型を持っていることを検証します。

.. configuration-block::

    .. code-block:: yaml

        properties:
            age:
                - Type: integer

    .. code-block:: php-annotations

        /**
         * @Assert\Type(type="integer")
         */
       protected $age;


オプション
----------

* ``type`` (**デフォルト**, 必須): 完全修飾クラス名またはいずれかの
  PHPのデータ型は、PHPの``のis_``関数によって決定されます。

  * `array <http://php.net/is_array>`_
  * `bool <http://php.net/is_bool>`_
  * `callable <http://php.net/is_callable>`_
  * `float <http://php.net/is_float>`_ 
  * `double <http://php.net/is_double>`_
  * `int <http://php.net/is_int>`_ 
  * `integer <http://php.net/is_integer>`_
  * `long <http://php.net/is_long>`_
  * `null <http://php.net/is_null>`_
  * `numeric <http://php.net/is_numeric>`_
  * `object <http://php.net/is_object>`_
  * `real <http://php.net/is_real>`_
  * `resource <http://php.net/is_resource>`_
  * `scalar <http://php.net/is_scalar>`_
  * `string <http://php.net/is_string>`_
* ``message``: 検証に失敗した場合のエラーメッセージ
