AssertType
==========

.. Validates that a value has a specific data type

値が指定したデータ型であることのバリデーションを実行します。

.. code-block:: yaml

    properties:
        age:
            - AssertType: integer

オプション
----------

.. * ``type`` (**default**, required): A fully qualified class name or one of the
    PHP datatypes as determined by PHP's ``is_`` functions.

* ``type`` (**デフォルト**, 必須): 完全修飾のクラス名か、PHPの ``is_`` 関数で定められたPHPのデータ型の1つ。

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
.. * ``message``: The error message in case the validation fails

* ``message``: バリデーションが失敗した時のエラーメッセージ。
